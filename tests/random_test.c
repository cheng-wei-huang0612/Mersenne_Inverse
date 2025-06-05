// randtest_case123.c – stress-test harness for cpt_inv()
// 編譯：  gcc -O2 randtest_case123.c big30.c big30_mpz.c cpt_inv.S -lgmp -o randtest
// 使用：  ./randtest [NUM_TESTS] [SEED]
//   NUM_TESTS – 預設 10000 ；可自行調大作壓力測試
//   SEED      – 若給定則固定 PRNG，方便 CI 重現
//
// 輸出判別系統：對每筆測資按下列三類統計
//   Case-1  canonical   – 你的結果 == mpz_invert 標準代表
//   Case-2  non-canonical – (yourInv − refInv) ≡ 0 (mod P) 但兩值不同
//   Case-3  wrong        – (x * yourInv) mod P ≠ 1   ➜  反元素錯誤
//
//  最終列印三類計數、失敗範例（限前 10 筆），並以非 0 exit code 代表有 Case-3。
//
// 需要 big30.h / big30_mpz.h / cpt_inv.h 與 GMP。

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <gmp.h>
extern void uint256_from_mpz(uint64_t dst[4], const mpz_t src);
extern void cpt_inv(uint64_t *x, uint64_t *inv);
extern void mpz_from_uint256(mpz_t dst, const uint64_t src[4]);


#define MAX_SAVE 10   /* 最多列印 10 筆範例 */

/* 保存錯誤或非 canonical 測資 */
static void save_hex(char *store[MAX_SAVE], size_t *cnt, const mpz_t z) {
    if (*cnt >= MAX_SAVE) return;
    store[*cnt] = mpz_get_str(NULL, 16, z);
    (*cnt)++;
}

int main(int argc, char **argv) {
    /* -------- 參數與 PRNG -------- */
    long num_tests   = (argc > 1) ? strtol(argv[1], NULL, 10) : 10000;
    unsigned long sd = (argc > 2) ? strtoul(argv[2], NULL, 10) : (unsigned long)time(NULL);

    printf("Running %ld tests – seed %lu\n", num_tests, sd);

    gmp_randstate_t rs; gmp_randinit_default(rs); gmp_randseed_ui(rs, sd);

    /* -------- 常量 P -------- */
    mpz_t mpP; mpz_init(mpP);
    mpz_set_ui(mpP, 1);
    mpz_mul_2exp(mpP, mpP, 255);
    mpz_sub_ui(mpP, mpP, 19);           /* P = 2^255 − 19 */

    /* -------- 循環內變量 -------- */
    mpz_t mpX, mpInv, mpInvRef, mpCheck, mpDiff;
    mpz_inits(mpX, mpInv, mpInvRef, mpCheck, mpDiff, NULL);
    uint64_t x[4], xInv[4];

    /* -------- 統計 -------- */
    long case1_cnt = 0, case2_cnt = 0, case3_cnt = 0;
    char *noncanon_hex[MAX_SAVE] = {0}; size_t noncanon_n = 0;
    char *fail_hex[MAX_SAVE]     = {0}; size_t fail_n = 0;

    for (long i = 0; i < num_tests; i++) {
        /* ---- 產生測資 ---- */
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

        /* ---- 呼叫 cpt_inv ---- */
        cpt_inv(x, xInv);
        mpz_from_uint256(mpInv, xInv);

        /* ---- 驗證 product ---- */
        mpz_mul(mpCheck, mpX, mpInv);
        mpz_mod(mpCheck, mpCheck, mpP);
        if (mpz_cmp_ui(mpCheck, 1) != 0) {
            /* Case-3 */
            case3_cnt++;
            save_hex(fail_hex, &fail_n, mpX);
            continue;
        }

        /* ---- 與 canonical inverse 比對 ---- */
        mpz_invert(mpInvRef, mpX, mpP);      /* gmp 保證 0 < ref < P */
        if (mpz_cmp(mpInv, mpInvRef) == 0) {
            case1_cnt++;                     /* 完全符合 */
        } else {
            /* 檢查是否僅差 k·P */
            mpz_sub(mpDiff, mpInv, mpInvRef);
            mpz_mod(mpDiff, mpDiff, mpP);
            if (mpz_cmp_ui(mpDiff, 0) == 0) {
                case2_cnt++;                 /* 非 canonical */
                save_hex(noncanon_hex, &noncanon_n, mpX);
            } else {
                /* 應該不會發生，當作 Case-3 */
                case3_cnt++;
                save_hex(fail_hex, &fail_n, mpX);
            }
        }
    }

    /* -------- 結果報告 -------- */
    printf("\n==== SUMMARY ====\n");
    printf("Case-1 canonical      : %ld\n", case1_cnt);
    printf("Case-2 non-canonical  : %ld\n", case2_cnt);
    printf("Case-3 WRONG (error) : %ld\n", case3_cnt);

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

    /* -------- 清理 -------- */
    for (size_t i = 0; i < noncanon_n; i++) free(noncanon_hex[i]);
    for (size_t i = 0; i < fail_n; i++)     free(fail_hex[i]);
    mpz_clears(mpDiff, mpCheck, mpInvRef, mpInv, mpX, mpP, NULL);

    if (case3_cnt) {
        printf("\n*** THERE ARE %ld WRONG RESULTS! ***\n", case3_cnt);
        return 1;
    }

    puts("\nAll tests passed (within canonical / non-canonical distinction).");
    return 0;
}