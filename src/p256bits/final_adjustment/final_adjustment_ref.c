#include <stdint.h>
#include <gmp.h>
#include "../slot.h"
/* 以 30-bit limb 讀 tmp → 填 mpz */
static void mpz_from_tmp_big30(mpz_t z, const uint64_t *T, int base_idx)
{
    mpz_set_ui(z, 0);
    mpz_set_si(z, (int64_t)(int32_t)(T[base_idx + 8] & 0xFFFFFFFF)); /* limb[8] 低 32 bit */
    for (int i = 8-1; i >= 0; i--) {
        mpz_mul_2exp(z, z, 30);                 /* << 30  */
        mpz_add_ui(z, z, T[base_idx + i]);
    }
}

/* mpz → 回寫到 tmp[base]…tmp[base+8]（低 limb 在前） */
static void tmp_big30_from_mpz(uint64_t *T, int base_idx, const mpz_t z)
{
    mpz_t tmp;  mpz_init_set(tmp, z);
    for (int i = 0; i < 8; i++) {
        T[base_idx + i] = mpz_get_si(tmp) & 0x3FFFFFFF;
        mpz_fdiv_q_2exp(tmp, tmp, 30);          /* >> 30  */
    }
    T[base_idx + 8] = mpz_get_si(tmp) & 0xFFFFFFFF;
    mpz_clear(tmp);
}
void final_adjustment(uint64_t *tmp) {

    tmp[IDX_VEC_V0] = tmp[IDX_VEC_V0_V1_S0_S1] & 0xFFFFFFFF; 
    tmp[IDX_VEC_V1] = (tmp[IDX_VEC_V0_V1_S0_S1] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_S0] = tmp[IDX_VEC_V0_V1_S0_S1_HI1] & 0xFFFFFFFF;
    tmp[IDX_VEC_S1] = (tmp[IDX_VEC_V0_V1_S0_S1_HI1] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_V2] = tmp[IDX_VEC_V2_V3_S2_S3] & 0xFFFFFFFF;
    tmp[IDX_VEC_V3] = (tmp[IDX_VEC_V2_V3_S2_S3] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_S2] = tmp[IDX_VEC_V2_V3_S2_S3_HI1] & 0xFFFFFFFF;
    tmp[IDX_VEC_S3] = (tmp[IDX_VEC_V2_V3_S2_S3_HI1] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_V4] = tmp[IDX_VEC_V4_V5_S4_S5] & 0xFFFFFFFF;
    tmp[IDX_VEC_V5] = (tmp[IDX_VEC_V4_V5_S4_S5] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_S4] = tmp[IDX_VEC_V4_V5_S4_S5_HI1] & 0xFFFFFFFF;
    tmp[IDX_VEC_S5] = (tmp[IDX_VEC_V4_V5_S4_S5_HI1] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_V6] = tmp[IDX_VEC_V6_V7_S6_S7] & 0xFFFFFFFF;
    tmp[IDX_VEC_V7] = (tmp[IDX_VEC_V6_V7_S6_S7] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_S6] = tmp[IDX_VEC_V6_V7_S6_S7_HI1] & 0xFFFFFFFF;
    tmp[IDX_VEC_S7] = (tmp[IDX_VEC_V6_V7_S6_S7_HI1] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_V8] = tmp[IDX_VEC_V8_V9_S8_S9] & 0xFFFFFFFF;
    //tmp[IDX_VEC_V9] = (tmp[IDX_VEC_V8_V9_S8_S9] >> 30) & 0xFFFFFFFF;
    tmp[IDX_VEC_S8] = tmp[IDX_VEC_V8_V9_S8_S9_HI1] & 0xFFFFFFFF;
    //tmp[IDX_VEC_S9] = (tmp[IDX_VEC_V8_V9_S8_S9_HI1] >> 30) & 0xFFFFFFFF;


    tmp[IDX_VEC_F0] = tmp[IDX_VEC_F0_F1_G0_G1] & 0xFFFFFFFF; 
    tmp[IDX_VEC_F1] = (tmp[IDX_VEC_F0_F1_G0_G1] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_G0] = tmp[IDX_VEC_F0_F1_G0_G1_HI1] & 0xFFFFFFFF;
    tmp[IDX_VEC_G1] = (tmp[IDX_VEC_F0_F1_G0_G1_HI1] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_F2] = tmp[IDX_VEC_F2_F3_G2_G3] & 0xFFFFFFFF;
    tmp[IDX_VEC_F3] = (tmp[IDX_VEC_F2_F3_G2_G3] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_G2] = tmp[IDX_VEC_F2_F3_G2_G3_HI1] & 0xFFFFFFFF;
    tmp[IDX_VEC_G3] = (tmp[IDX_VEC_F2_F3_G2_G3_HI1] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_F4] = tmp[IDX_VEC_F4_F5_G4_G5] & 0xFFFFFFFF;
    tmp[IDX_VEC_F5] = (tmp[IDX_VEC_F4_F5_G4_G5] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_G4] = tmp[IDX_VEC_F4_F5_G4_G5_HI1] & 0xFFFFFFFF;
    tmp[IDX_VEC_G5] = (tmp[IDX_VEC_F4_F5_G4_G5_HI1] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_F6] = tmp[IDX_VEC_F6_F7_G6_G7] & 0xFFFFFFFF;
    tmp[IDX_VEC_F7] = (tmp[IDX_VEC_F6_F7_G6_G7] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_G6] = tmp[IDX_VEC_F6_F7_G6_G7_HI1] & 0xFFFFFFFF;
    tmp[IDX_VEC_G7] = (tmp[IDX_VEC_F6_F7_G6_G7_HI1] >> 32) & 0xFFFFFFFF;
    tmp[IDX_VEC_F8] = tmp[IDX_VEC_F8_F9_G8_G9] & 0xFFFFFFFF;
    //tmp[IDX_VEC_F9] = (tmp[IDX_VEC_F8_F9_G8_G9] >> 30) & 0xFFFFFFFF;
    tmp[IDX_VEC_G8] = tmp[IDX_VEC_F8_F9_G8_G9_HI1] & 0xFFFFFFFF;
    //tmp[IDX_VEC_G9] = (tmp[IDX_VEC_F8_F9_G8_G9_HI1] >> 30) & 0xFFFFFFFF;


    /* 1. 轉 V → mpz */
    mpz_t mpV;      mpz_init(mpV);
    mpz_from_tmp_big30(mpV, tmp, IDX_VEC_V0);

    /* 2. 取 sign(F)  (limb0 最低 30 bit，帶符號延伸到 32 bit) */
    int32_t sign_F = (int32_t)tmp[IDX_VEC_F0];   /* 取 32 bit  */
    sign_F <<= 2;   sign_F >>= 2;            /* 保留最低 30 bit 的符號 */

    /* 3. 若 sign_F 為 −1 則取模補正；若 0/1 則照乘 */
    mpz_t mpSign;   mpz_init_set_si(mpSign, sign_F);

    /* 4. 模數 P（靜態一次性建） */
    static mpz_t mpP;
    mpz_init(mpP);
    mpz_set_ui(mpP, 0);
    for (int i = 8; i >= 0; i--) {
        mpz_add_ui(mpP, mpP, tmp[IDX_VEC_P0 + i]);
        if (i != 0) {
            mpz_mul_2exp(mpP, mpP, 30);
        }
    }
    // gmp_printf("mpP        = %Zd\n", mpP);

    /* 5. V ← V * sign(F)  (mod P) */
    mpz_mul(mpV, mpV, mpSign);
    mpz_mod(mpV, mpV, mpP);

    /* 6. 回寫到 tmp[] */
    tmp_big30_from_mpz(tmp, IDX_VEC_V0, mpV);


    uint64_t *inv = (uint64_t *)&tmp[IDX_INV0];

    for (int i = 8; i >= 0; i--) {
        // 整個 t[3..0] << 30
        // new_t[3] = (old_t[3] << 30) | (old_t[2] >> (64-30))
        // new_t[2] = (old_t[2] << 30) | (old_t[1] >> (64-30))
        // ... 類似 shiftLeft30，這裡直接手動 inline

        uint64_t old0 = inv[0];
        uint64_t old1 = inv[1];
        uint64_t old2 = inv[2];
        uint64_t old3 = inv[3];

        inv[3] = (old3 << 30) | (old2 >> 34);
        inv[2] = (old2 << 30) | (old1 >> 34);
        inv[1] = (old1 << 30) | (old0 >> 34);
        inv[0] = (old0 << 30);

        // 再加上 x->limb[i] (最低 30 bits)，以 64-bit 來裝沒問題
        inv[0] |= (uint64_t) tmp[IDX_VEC_V0 + i] & 0x3FFFFFFF; 
    }


    mpz_clears(mpV, mpSign, NULL);
}