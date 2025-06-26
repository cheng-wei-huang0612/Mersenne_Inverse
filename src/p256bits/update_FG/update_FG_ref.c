#include <stdint.h>
#include <stdio.h>
#include <arm_neon.h>
#include "../slot.h"
#include <gmp.h>
#include <inttypes.h>

static inline void print_u32x4(const char *tag, uint32x4_t v)
{
    uint32_t buf[4];          // 4×32-bit = 128-bit
    vst1q_u32(buf, v);        // 將向量存回一般記憶體
    printf("%s = 0x%08" PRIx32 " 0x%08" PRIx32
                  " 0x%08" PRIx32 " 0x%08" PRIx32 "\n",
           tag, buf[0], buf[1], buf[2], buf[3]);
}




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


    mpz_from_tmp_big30(mpF, tmp, IDX_VEC_F0);
    mpz_from_tmp_big30(mpG, tmp, IDX_VEC_G0);

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
    tmp_big30_from_mpz(tmp, IDX_VEC_F0, mpF_new);
    tmp_big30_from_mpz(tmp, IDX_VEC_G0, mpG_new);

    tmp[IDX_VEC_F0_F1_G0_G1] = (tmp[IDX_VEC_F0] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_F1] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F0_F1_G0_G1_HI1] = (tmp[IDX_VEC_G0] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_G1] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F2_F3_G2_G3] = (tmp[IDX_VEC_F2] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_F3] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F2_F3_G2_G3_HI1] = (tmp[IDX_VEC_G2] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_G3] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F4_F5_G4_G5] = (tmp[IDX_VEC_F4] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_F5] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F4_F5_G4_G5_HI1] = (tmp[IDX_VEC_G4] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_G5] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F6_F7_G6_G7] = (tmp[IDX_VEC_F6] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_F7] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F6_F7_G6_G7_HI1] = (tmp[IDX_VEC_G6] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_G7] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F8_F9_G8_G9] = (tmp[IDX_VEC_F8] & 0xFFFFFFFF) ;
    tmp[IDX_VEC_F8_F9_G8_G9_HI1] = (tmp[IDX_VEC_G8] & 0xFFFFFFFF) ;
    
    int32x4_t VEC_F0_F1_G0_G1 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_F0_F1_G0_G1]);
    int32x4_t VEC_F2_F3_G2_G3 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_F2_F3_G2_G3]);
    int32x4_t VEC_F4_F5_G4_G5 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_F4_F5_G4_G5]);
    int32x4_t VEC_F6_F7_G6_G7 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_F6_F7_G6_G7]);
    int32x4_t VEC_F8_F9_G8_G9 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_F8_F9_G8_G9]);

    print_u32x4("VEC_F0_F1_G0_G1", (uint32x4_t)VEC_F0_F1_G0_G1);
    print_u32x4("VEC_F2_F3_G2_G3", (uint32x4_t)VEC_F2_F3_G2_G3);
    print_u32x4("VEC_F4_F5_G4_G5", (uint32x4_t)VEC_F4_F5_G4_G5);
    print_u32x4("VEC_F6_F7_G6_G7", (uint32x4_t)VEC_F6_F7_G6_G7);
    print_u32x4("VEC_F8_F9_G8_G9", (uint32x4_t)VEC_F8_F9_G8_G9);
    printf("-------------------------------------------------------------\n");


    /* 5. 清理 */
    mpz_clears(mpF, mpG, mpF_new, mpG_new,
            mpuu, mpvv, mprr, mpss, acc, NULL);
}

