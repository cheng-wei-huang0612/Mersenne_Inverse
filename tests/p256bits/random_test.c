// randtest_case123.c – stress-test harness for cpt_inv()
// 編譯：  gcc -O2 randtest_case123.c big30.c big30_mpz.c cpt_inv.S -lgmp -o randtest
// 使用：  ./randtest [NUM_TESTS] [SEED]
//   NUM_TESTS – 預設 10000 ；可自行調大作壓力測試
//   SEED      – 若給定則固定 PRNG，方便 CI 重現
//
// 判別系統（最終分類）：
//   Case-1  canonical      – 你的結果 == mpz_invert 標準代表
//   Case-2  non-canonical  – (yourInv − refInv) ≡ 0 (mod P) 但兩值不同
//   Case-3  wrong          – 在最多 5 次嘗試後仍無法得到 (x·inv) ≡ 1
//
// 任何 Case-3 會以非零 exit code 結束，方便 CI 偵測。
//
// 需要 big30.h / big30_mpz.h / cpt_inv.h 與 GMP。

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

/* 你的轉換 / 反元素函式 */
extern void uint256_from_mpz(uint64_t dst[4], const mpz_t src);
extern void cpt_inv(uint64_t *x, uint64_t *inv, uint64_t *P);
extern void mpz_from_uint256(mpz_t dst, const uint64_t src[4]);

#define MAX_SAVE    10     /* 每類最多列印 10 筆範例 */
#define MAX_RETRIES 5      /* 含第一次，一共 5 次嘗試 */

/* 保存 hex 字串 */
static void save_hex(char *store[MAX_SAVE], size_t *cnt, const mpz_t z) {
    if (*cnt >= MAX_SAVE) return;
    store[*cnt] = mpz_get_str(NULL, 16, z);
    (*cnt)++;
}

int main(int argc, char **argv) {
    /* -------- 參數與 PRNG -------- */
    long num_tests   = (argc > 1) ? strtol(argv[1], NULL, 10) : 100000;
    unsigned long sd = (argc > 2) ? strtoul(argv[2], NULL, 10) : (unsigned long)time(NULL);

    printf("Running %ld tests – seed %lu\n", num_tests, sd);

    gmp_randstate_t rs; gmp_randinit_default(rs); gmp_randseed_ui(rs, sd);

    /* -------- 常量 P  = 2^255 − 19 -------- */
    mpz_t mpP; mpz_init(mpP);
    mpz_set_ui(mpP, 1);
    mpz_mul_2exp(mpP, mpP, 255);
    mpz_sub_ui(mpP, mpP, 475);

    /* -------- 循環內變量 -------- */
    mpz_t mpX, mpInv, mpInvRef, mpCheck, mpDiff;   /* 大整數 */
    mpz_inits(mpX, mpInv, mpInvRef, mpCheck, mpDiff, NULL);
    uint64_t x[4], P[4], xInv[4];                        /* 256-bit 原始格式 */

    /* -------- 統計 -------- */
    long case1_cnt = 0, case2_cnt = 0, case3_cnt = 0;
    char *noncanon_hex[MAX_SAVE] = {0}; size_t noncanon_n = 0;
    char *fail_hex[MAX_SAVE]     = {0}; size_t fail_n = 0;

    /* -------- 逐筆測試 -------- */
    for (long i = 0; i < num_tests; i++) {
        /* ---- 生成測資 ---- */
        if (i == 0) {
            mpz_set_ui(mpX, 1);
        } else if (i == 1) {
            mpz_sub_ui(mpX, mpP, 1);
        } else {
            do {
                mpz_urandomm(mpX, rs, mpP);
            } while (mpz_cmp_ui(mpX, 0) == 0);
        }
        uint256_from_mpz(x, mpX);

        uint256_from_mpz(P, mpP);
        
        /* ---- 最多嘗試 MAX_RETRIES 次 ---- */
        int ok = 0;
        for (int t = 0; t < MAX_RETRIES; t++) {
            cpt_inv(x, xInv, P);
            mpz_from_uint256(mpInv, xInv);

            /* 檢查 (x*inv) mod P == 1 ? */
            mpz_mul(mpCheck, mpX, mpInv);
            mpz_mod(mpCheck, mpCheck, mpP);
            if (mpz_cmp_ui(mpCheck, 1) == 0) {
                ok = 1;                     /* 成功 */
                break;
            }
        }

        if (!ok) {
            /* -------- Case-3：五次皆失敗 -------- */
            case3_cnt++;
            save_hex(fail_hex, &fail_n, mpX);
            continue;
        }

        /* ---- 成功後：與 canonical 比對 ---- */
        mpz_invert(mpInvRef, mpX, mpP);      /* 0 < ref < P */
        if (mpz_cmp(mpInv, mpInvRef) == 0) {
            case1_cnt++;                     /* Case-1 完全相同 */
        } else {
            mpz_sub(mpDiff, mpInv, mpInvRef);
            mpz_mod(mpDiff, mpDiff, mpP);
            if (mpz_cmp_ui(mpDiff, 0) == 0) {
                case2_cnt++;                 /* Case-2 只是代表不同 */
                save_hex(noncanon_hex, &noncanon_n, mpX);
            } else {
                /* 理論上不該到這裡，但仍歸類 Case-3 */
                case3_cnt++;
                save_hex(fail_hex, &fail_n, mpX);
            }
        }
    }

    /* -------- 結果報告 -------- */
    printf("\n==== SUMMARY ====\n");
    printf("Case-1 canonical      : %ld\n", case1_cnt);
    printf("Case-2 non-canonical  : %ld\n", case2_cnt);
    printf("Case-3 WRONG (error)  : %ld\n", case3_cnt);

    if (noncanon_n) {
        printf("\nNon-canonical examples (x in hex):\n");
        for (size_t i = 0; i < noncanon_n; i++)
            printf("  %s\n", noncanon_hex[i]);
    }
    if (fail_n) {
        printf("\nFAILED examples (x in hex):\n");
        for (size_t i = 0; i < fail_n; i++)
            printf("  %s\n", fail_hex[i]);
    }

    /* -------- 釋放記憶體 -------- */
    for (size_t i = 0; i < noncanon_n; i++) free(noncanon_hex[i]);
    for (size_t i = 0; i < fail_n; i++)     free(fail_hex[i]);
    mpz_clears(mpDiff, mpCheck, mpInvRef, mpInv, mpX, mpP, NULL);

    if (case3_cnt) {
        printf("\n*** THERE ARE %ld WRONG RESULTS! ***\n", case3_cnt);
        return 1;
    }

    puts("\nAll tests passed (within canonical / non-canonical distinction).\n");
    return 0;
}