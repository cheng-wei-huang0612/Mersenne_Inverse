#include <stdint.h>
#include <stdio.h>
#include "slot.h"
#include <gmp.h>

/* 以 30-bit limb 讀 tmp → 填 mpz */
static inline void mpz_from_tmp_big30(mpz_t z, const uint64_t *T, int base_idx)
{
    mpz_set_ui(z, 0);
    mpz_set_si(z, (int64_t)(int32_t)(T[base_idx + 8] & 0xFFFFFFFF)); /* limb[8] 低 32 bit */
    for (int i = 8-1; i >= 0; i--) {
        mpz_mul_2exp(z, z, 30);                 /* << 30  */
        mpz_add_ui(z, z, T[base_idx + i]);
    }
}

/* mpz → 回寫到 tmp[base]…tmp[base+8]（低 limb 在前） */
static inline void tmp_big30_from_mpz(uint64_t *T, int base_idx, const mpz_t z)
{
    mpz_t tmp;  mpz_init_set(tmp, z);
    for (int i = 0; i < 8; i++) {
        T[base_idx + i] = mpz_get_si(tmp) & 0x3FFFFFFF;
        mpz_fdiv_q_2exp(tmp, tmp, 30);          /* >> 30  */
    }
    T[base_idx + 8] = mpz_get_si(tmp) & 0xFFFFFFFF;
    mpz_clear(tmp);
}

void update_VS(uint64_t *tmp) {
    /* 讀 uu vv rr ss (64-bit) ----------------------------------- */
    int64_t uu = (int64_t)tmp[IDX_UU];
    int64_t vv = (int64_t)tmp[IDX_VV];
    int64_t rr = (int64_t)tmp[IDX_RR];
    int64_t ss = (int64_t)tmp[IDX_SS];

    /* ----------- mpz 變數宣告 / 轉入 --------------------------- */
    mpz_t mpV, mpS;
    mpz_inits(mpV, mpS, NULL);
    mpz_from_tmp_big30(mpV, tmp, IDX_V0);
    mpz_from_tmp_big30(mpS, tmp, IDX_S0);

    mpz_t mpuu, mpvv, mprr, mpss;
    mpz_inits(mpuu, mpvv, mprr, mpss, NULL);
    mpz_set_si(mpuu, uu);
    mpz_set_si(mpvv, vv);
    mpz_set_si(mprr, rr);
    mpz_set_si(mpss, ss);

    /* ----------- 模數 P (全域 big30_t P → mpz，一次性建立即可) */
    static mpz_t mpP;
    mpz_init(mpP);
    mpz_set_ui(mpP, 1);
    mpz_mul_2exp(mpP, mpP, 255); // P = 2^255
    mpz_sub_ui(mpP, mpP, 19); // P = 2^255 - 19


    /* ----------- (2^60)^-1 (mod P) 只算一次 ------------------- */
    static mpz_t mpInv2p60;
    static int   inv_ready = 0;
    if (!inv_ready) {
        mpz_init_set_ui(mpInv2p60, 1);
        mpz_mul_2exp(mpInv2p60, mpInv2p60, 60);  /* 2^60          */
        mpz_invert(mpInv2p60, mpInv2p60, mpP);   /* inverse mod P */
        inv_ready = 1;
    }

    /* ----------- V_new = (uu·V + vv·S) * inv2p60 mod P -------- */
    mpz_t tmp1;  mpz_init(tmp1);
    mpz_mul(tmp1, mpuu, mpV);
    mpz_addmul(tmp1, mpvv, mpS);
    mpz_mul(tmp1, tmp1, mpInv2p60);
    mpz_mod(tmp1, tmp1, mpP);         /* tmp1 = V_new */

    /* ----------- S_new = (rr·V + ss·S) * inv2p60 mod P -------- */
    mpz_t tmp2;  mpz_init(tmp2);
    mpz_mul(tmp2, mprr, mpV);
    mpz_addmul(tmp2, mpss, mpS);
    mpz_mul(tmp2, tmp2, mpInv2p60);
    mpz_mod(tmp2, tmp2, mpP);         /* tmp2 = S_new */

    /* ----------- 回寫到 tmp[] ---------------------------------- */
    tmp_big30_from_mpz(tmp, IDX_V0, tmp1);
    tmp_big30_from_mpz(tmp, IDX_S0, tmp2);

    /* ----------- 清理 (除 mpP/mpInv2p60) ---------------------- */
    mpz_clears(mpV, mpS, mpuu, mpvv, mprr, mpss, tmp1, tmp2, NULL);
}

