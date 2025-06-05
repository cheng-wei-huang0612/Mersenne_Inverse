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

void update_FG(uint64_t *tmp) {
        /* 1. 轉 F,G → mpz */
    mpz_t mpF, mpG;                      mpz_inits(mpF, mpG, NULL);
    mpz_from_tmp_big30(mpF, tmp, IDX_F0);
    mpz_from_tmp_big30(mpG, tmp, IDX_G0);

    /* 2. 係數 uu vv rr ss */
    mpz_t mpuu, mpvv, mprr, mpss;        mpz_inits(mpuu, mpvv, mprr, mpss, NULL);
    mpz_set_si(mpuu, (int64_t)tmp[IDX_UU]);
    mpz_set_si(mpvv, (int64_t)tmp[IDX_VV]);
    mpz_set_si(mprr, (int64_t)tmp[IDX_RR]);
    mpz_set_si(mpss, (int64_t)tmp[IDX_SS]);

    /* 3. F_new & G_new  (>>60) */
    mpz_t acc, mpF_new, mpG_new;         mpz_inits(acc, mpF_new, mpG_new, NULL);

    mpz_mul(acc, mpuu, mpF);             /* uu*F            */
    mpz_addmul(acc, mpvv, mpG);          /* + vv*G          */
    mpz_fdiv_q_2exp(mpF_new, acc, 60);   /* >> 60           */

    mpz_mul(acc, mprr, mpF);             /* rr*F            */
    mpz_addmul(acc, mpss, mpG);          /* + ss*G          */
    mpz_fdiv_q_2exp(mpG_new, acc, 60);   /* >> 60           */

    /* 4. 回寫到 tmp[] */
    tmp_big30_from_mpz(tmp, IDX_F0, mpF_new);
    tmp_big30_from_mpz(tmp, IDX_G0, mpG_new);

    /* 5. 清理 */
    mpz_clears(mpF, mpG, mpF_new, mpG_new,
            mpuu, mpvv, mprr, mpss, acc, NULL);
}

