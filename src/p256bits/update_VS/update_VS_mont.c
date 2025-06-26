#include <stdint.h>
#include <stdio.h>
#include "../slot.h"
//#include <gmp.h>
#include <arm_neon.h>
#include <inttypes.h>
#include <assert.h>


// static inline void print_u32x4(const char *tag, uint32x4_t v)
// {
//     uint32_t buf[4];          // 4×32-bit = 128-bit
//     vst1q_u32(buf, v);        // 將向量存回一般記憶體
//     printf("%s = 0x%08" PRIx32 " 0x%08" PRIx32
//                   " 0x%08" PRIx32 " 0x%08" PRIx32 "\n",
//            tag, buf[0], buf[1], buf[2], buf[3]);
// }




// /* 以 30-bit limb 讀 tmp → 填 mpz */
// static inline void mpz_from_tmp_big30(mpz_t z, const uint64_t *T, int base_idx)
// {
//     mpz_set_ui(z, 0);
//     mpz_set_si(z, (int64_t)(int32_t)(T[base_idx + 8] & 0xFFFFFFFF)); /* limb[8] 低 32 bit */
//     for (int i = 8-1; i >= 0; i--) {
//         mpz_mul_2exp(z, z, 30);                 /* << 30  */
//         mpz_add_ui(z, z, T[base_idx + i]);
//     }
// }

// /* mpz → 回寫到 tmp[base]…tmp[base+8]（低 limb 在前） */
// static inline void tmp_big30_from_mpz(uint64_t *T, int base_idx, const mpz_t z)
// {
//     mpz_t tmp;  mpz_init_set(tmp, z);
//     for (int i = 0; i < 8; i++) {
//         T[base_idx + i] = mpz_get_si(tmp) & 0x3FFFFFFF;
//         mpz_fdiv_q_2exp(tmp, tmp, 30);          /* >> 30  */
//     }
//     T[base_idx + 8] = mpz_get_si(tmp) & 0xFFFFFFFF;
//     mpz_clear(tmp);
// }

void update_VS(uint64_t *tmp) {
    /* 讀 uu vv rr ss (64-bit) ----------------------------------- */
    int64_t uu = (int64_t)tmp[IDX_UU];
    int64_t vv = (int64_t)tmp[IDX_VV];
    int64_t rr = (int64_t)tmp[IDX_RR];
    int64_t ss = (int64_t)tmp[IDX_SS];


    int32_t *vec0 = (int32_t *)&tmp[IDX_VEC_UU0_RR0_VV0_SS0];
    int32_t *vec1 = (int32_t *)&tmp[IDX_VEC_UU1_RR1_VV1_SS1];

    const int64_t mask30 = (1LL << 30) - 1;   /* 0x3FFFFFFF，非巨集 */

    vec0[0] = (int32_t)(uu & mask30);
    vec0[1] = (int32_t)(rr & mask30);
    vec0[2] = (int32_t)(vv & mask30);
    vec0[3] = (int32_t)(ss & mask30);

    vec1[0] = (int32_t)(uu >> 30);   /* C 在主流編譯器上對有號值採算術右移 */
    vec1[1] = (int32_t)(rr >> 30);
    vec1[2] = (int32_t)(vv >> 30);
    vec1[3] = (int32_t)(ss >> 30);
    

    #define MASK30  0x3FFFFFFF

    uint32_t p0 = tmp[IDX_VEC_P0];
    uint32_t inv = tmp[IDX_VEC_P0];

    inv *= 2 - p0 * inv;   /* 2 bit */
    inv *= 2 - p0 * inv;   /* 4 bit */
    inv *= 2 - p0 * inv;   /* 8 bit */
    inv *= 2 - p0 * inv;   /* 16 bit */
    inv *= 2 - p0 * inv;   /* 32 bit ≥ 30 */
    inv &= MASK30;

    /* 先確認正逆元：p0 * inv ≡ 1 (mod 2^30) */
    uint64_t prod1 = (uint64_t)p0 * inv;
    assert((prod1 & MASK30) == 1);

    /* -------- 2. 取負號得到 Montgomery 常數 M = -P^{-1} mod 2^30 -------- */
    uint32_t M = (0x40000000 - inv) & MASK30;    /* 也可寫 (MASK30 + 1 - inv) & MASK30 */
    int32x4_t VEC_M = vdupq_n_s32((int32_t)M);

    /* 驗證負逆元：p0 * M ≡ -1 ≡ 2^30-1 (mod 2^30) */
    uint64_t prod2 = (uint64_t)p0 * M;
    assert((prod2 & MASK30) == MASK30);



    int32x4_t VEC_P0_P1_P2_P3 = {
        (int32_t) tmp[IDX_VEC_P0],
        (int32_t) tmp[IDX_VEC_P1],
        (int32_t) tmp[IDX_VEC_P2],
        (int32_t) tmp[IDX_VEC_P3]
    };

    int32x4_t VEC_P4_P5_P6_P7 = {
        (int32_t) tmp[IDX_VEC_P4],
        (int32_t) tmp[IDX_VEC_P5],
        (int32_t) tmp[IDX_VEC_P6],
        (int32_t) tmp[IDX_VEC_P7]
    };

    int32x4_t VEC_P8_0_0_0 = {
        (int32_t) tmp[IDX_VEC_P8],
        (int32_t) 0,
        (int32_t) 0,
        (int32_t) 0
    };

    int32x4_t VEC_CONST_2X_2P30M1 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_CONST_2X_2P30M1]);
    int32x4_t VEC_CONST_2X_2P32M1 = (int32x4_t)vdupq_n_s64(((int64_t)1<<32) - 1);

    
    int32x4_t VEC_V0_V1_S0_S1 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_V0_V1_S0_S1]);
    int32x4_t VEC_V2_V3_S2_S3 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_V2_V3_S2_S3]);
    int32x4_t VEC_V4_V5_S4_S5 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_V4_V5_S4_S5]);
    int32x4_t VEC_V6_V7_S6_S7 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_V6_V7_S6_S7]);
    int32x4_t VEC_V8_V9_S8_S9 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_V8_V9_S8_S9]);


    int32x4_t VEC_UU0_RR0_VV0_SS0 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_UU0_RR0_VV0_SS0]);
    int32x4_t VEC_UU1_RR1_VV1_SS1 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_UU1_RR1_VV1_SS1]);

    int64x2_t VEC_PROD = {0};
    int32x4_t VEC_BUFFER = {0};


    // limb 0
    VEC_PROD = vmull_laneq_s32(vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_V0_V1_S0_S1, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD,vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_V0_V1_S0_S1, 2);


    int32x4_t VEC_L0 = vmulq_s32((int32x4_t)VEC_PROD, VEC_M);
    
    VEC_L0 = vandq_s32(VEC_L0, VEC_CONST_2X_2P30M1);
    VEC_L0 = vuzp1q_s32(VEC_L0, VEC_L0);

    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L0), VEC_P0_P1_P2_P3, 0);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);



    // limb 1
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_V0_V1_S0_S1, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_V0_V1_S0_S1, 3);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_V0_V1_S0_S1, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_V0_V1_S0_S1, 2);

    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L0), VEC_P0_P1_P2_P3, 1);


    int32x4_t VEC_L1 = vmulq_s32((int32x4_t)VEC_PROD, VEC_M);

    VEC_L1 = vandq_s32(VEC_L1, VEC_CONST_2X_2P30M1);
    VEC_L1 = vuzp1q_s32(VEC_L1, VEC_L1);

    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L1), VEC_P0_P1_P2_P3, 0);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);
    



    // limb 2
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_V2_V3_S2_S3, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_V2_V3_S2_S3, 2);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_V0_V1_S0_S1, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_V0_V1_S0_S1, 3);

    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L0), VEC_P0_P1_P2_P3, 2);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L1), VEC_P0_P1_P2_P3, 1);

    VEC_V0_V1_S0_S1 = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);

    
    // limb 3
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_V2_V3_S2_S3, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_V2_V3_S2_S3, 3);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_V2_V3_S2_S3, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_V2_V3_S2_S3, 2);
    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L0), VEC_P0_P1_P2_P3, 3);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L1), VEC_P0_P1_P2_P3, 2);
    
    VEC_BUFFER = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);
    VEC_BUFFER = vshlq_n_s64((int64x2_t)VEC_BUFFER, 32);
    VEC_V0_V1_S0_S1 = vorrq_s32(VEC_V0_V1_S0_S1,VEC_BUFFER);


    // limb 4
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_V4_V5_S4_S5, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_V4_V5_S4_S5, 2);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_V2_V3_S2_S3, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_V2_V3_S2_S3, 3);

    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L0), VEC_P4_P5_P6_P7, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L1), VEC_P0_P1_P2_P3, 3);

    VEC_V2_V3_S2_S3 = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);

    
    // limb 5
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_V4_V5_S4_S5, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_V4_V5_S4_S5, 3);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_V4_V5_S4_S5, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_V4_V5_S4_S5, 2);
    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L0), VEC_P4_P5_P6_P7, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L1), VEC_P4_P5_P6_P7, 0);
    
    VEC_BUFFER = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);
    VEC_BUFFER = vshlq_n_s64((int64x2_t)VEC_BUFFER, 32);
    VEC_V2_V3_S2_S3 = vorrq_s32(VEC_V2_V3_S2_S3,VEC_BUFFER);


    // limb 6
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_V6_V7_S6_S7, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_V6_V7_S6_S7, 2);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_V4_V5_S4_S5, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_V4_V5_S4_S5, 3);

    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L0), VEC_P4_P5_P6_P7, 2);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L1), VEC_P4_P5_P6_P7, 1);

    VEC_V4_V5_S4_S5 = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);

    
    // limb 7
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_V6_V7_S6_S7, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_V6_V7_S6_S7, 3);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_V6_V7_S6_S7, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_V6_V7_S6_S7, 2);
    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L0), VEC_P4_P5_P6_P7, 3);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L1), VEC_P4_P5_P6_P7, 2);
    
    VEC_BUFFER = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);
    VEC_BUFFER = vshlq_n_s64((int64x2_t)VEC_BUFFER, 32);
    VEC_V4_V5_S4_S5 = vorrq_s32(VEC_V4_V5_S4_S5,VEC_BUFFER);

    // limb 8
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_V8_V9_S8_S9, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_V8_V9_S8_S9, 2);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_V6_V7_S6_S7, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_V6_V7_S6_S7, 3);

    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L0), VEC_P8_0_0_0, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L1), VEC_P4_P5_P6_P7, 3);

    VEC_V6_V7_S6_S7 = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);

    
    // limb 9
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_V8_V9_S8_S9, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_V8_V9_S8_S9, 2);
    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_L1), VEC_P8_0_0_0, 0);
    
    VEC_BUFFER = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);
    VEC_BUFFER = vshlq_n_s64((int64x2_t)VEC_BUFFER, 32);
    VEC_V6_V7_S6_S7 = vorrq_s32(VEC_V6_V7_S6_S7,VEC_BUFFER);

    VEC_V8_V9_S8_S9 = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P32M1);




    int32x4_t VEC_P0_P1_P0_P1 = (int32x4_t)vzip1q_s64((int64x2_t)VEC_P0_P1_P2_P3, (int64x2_t)VEC_P0_P1_P2_P3);
    int32x4_t VEC_P2_P3_P2_P3 = (int32x4_t)vzip2q_s64((int64x2_t)VEC_P0_P1_P2_P3, (int64x2_t)VEC_P0_P1_P2_P3);
    int32x4_t VEC_P4_P5_P4_P5 = (int32x4_t)vzip1q_s64((int64x2_t)VEC_P4_P5_P6_P7, (int64x2_t)VEC_P4_P5_P6_P7);
    int32x4_t VEC_P6_P7_P6_P7 = (int32x4_t)vzip2q_s64((int64x2_t)VEC_P4_P5_P6_P7, (int64x2_t)VEC_P4_P5_P6_P7);
    int32x4_t VEC_P8_0_P8_0 = (int32x4_t)vzip1q_s64((int64x2_t)VEC_P8_0_0_0, (int64x2_t)VEC_P8_0_0_0);
    
    int32x4_t VEC_CARRY = {0};
    int32x4_t VEC_CONST_4X_2P30M1 = vdupq_n_s32((int32_t)tmp[IDX_CONST_2P30M1]);
    int32x4_t VEC_CONST_CARRYS = vmvnq_s32(VEC_CONST_4X_2P30M1);

    // if V < 0 then add P once
    int64x2_t VEC_ADD_P_COND = vshrq_n_s64(VEC_PROD, 63);


    VEC_BUFFER = vandq_s32(VEC_P0_P1_P0_P1 ,(int32x4_t)VEC_ADD_P_COND);
    VEC_V0_V1_S0_S1 = vaddq_s32(VEC_V0_V1_S0_S1, VEC_BUFFER);
    VEC_CARRY = vbicq_s32(VEC_V0_V1_S0_S1, VEC_CONST_4X_2P30M1);
    VEC_CARRY = (int32x4_t)vshlq_n_s64((int64x2_t)VEC_CARRY, 2);
    VEC_V0_V1_S0_S1 = vaddq_s32(VEC_V0_V1_S0_S1, VEC_CARRY);
    VEC_CARRY = vbicq_s32(VEC_V0_V1_S0_S1, VEC_CONST_4X_2P30M1);
    VEC_CARRY = (int32x4_t)vshrq_n_s64((int64x2_t)VEC_CARRY, 62);
    VEC_V0_V1_S0_S1 = vbicq_s32(VEC_V0_V1_S0_S1, VEC_CONST_CARRYS);


    VEC_BUFFER = vandq_s32(VEC_P2_P3_P2_P3 ,(int32x4_t)VEC_ADD_P_COND);
    VEC_V2_V3_S2_S3 = vaddq_s32(VEC_V2_V3_S2_S3, VEC_BUFFER);
    VEC_V2_V3_S2_S3 = vaddq_s32(VEC_V2_V3_S2_S3, VEC_CARRY);
    VEC_CARRY = vbicq_s32(VEC_V2_V3_S2_S3, VEC_CONST_4X_2P30M1);
    VEC_CARRY = (int32x4_t)vshlq_n_s64((int64x2_t)VEC_CARRY, 2);
    VEC_V2_V3_S2_S3 = vaddq_s32(VEC_V2_V3_S2_S3, VEC_CARRY);
    VEC_CARRY = vbicq_s32(VEC_V2_V3_S2_S3, VEC_CONST_4X_2P30M1);
    VEC_CARRY = (int32x4_t)vshrq_n_s64((int64x2_t)VEC_CARRY, 62);
    VEC_V2_V3_S2_S3 = vbicq_s32(VEC_V2_V3_S2_S3, VEC_CONST_CARRYS);


    VEC_BUFFER = vandq_s32(VEC_P4_P5_P4_P5 ,(int32x4_t)VEC_ADD_P_COND);
    VEC_V4_V5_S4_S5 = vaddq_s32(VEC_V4_V5_S4_S5, VEC_BUFFER);
    VEC_V4_V5_S4_S5 = vaddq_s32(VEC_V4_V5_S4_S5, VEC_CARRY);
    VEC_CARRY = vbicq_s32(VEC_V4_V5_S4_S5, VEC_CONST_4X_2P30M1);
    VEC_CARRY = (int32x4_t)vshlq_n_s64((int64x2_t)VEC_CARRY, 2);
    VEC_V4_V5_S4_S5 = vaddq_s32(VEC_V4_V5_S4_S5, VEC_CARRY);
    VEC_CARRY = vbicq_s32(VEC_V4_V5_S4_S5, VEC_CONST_4X_2P30M1);
    VEC_CARRY = (int32x4_t)vshrq_n_s64((int64x2_t)VEC_CARRY, 62);
    VEC_V4_V5_S4_S5 = vbicq_s32(VEC_V4_V5_S4_S5, VEC_CONST_CARRYS);


    VEC_BUFFER = vandq_s32(VEC_P6_P7_P6_P7 ,(int32x4_t)VEC_ADD_P_COND);
    VEC_V6_V7_S6_S7 = vaddq_s32(VEC_V6_V7_S6_S7, VEC_BUFFER);
    VEC_V6_V7_S6_S7 = vaddq_s32(VEC_V6_V7_S6_S7, VEC_CARRY);
    VEC_CARRY = vbicq_s32(VEC_V6_V7_S6_S7, VEC_CONST_4X_2P30M1);
    VEC_CARRY = (int32x4_t)vshlq_n_s64((int64x2_t)VEC_CARRY, 2);
    VEC_V6_V7_S6_S7 = vaddq_s32(VEC_V6_V7_S6_S7, VEC_CARRY);
    VEC_CARRY = vbicq_s32(VEC_V6_V7_S6_S7, VEC_CONST_4X_2P30M1);
    VEC_CARRY = (int32x4_t)vshrq_n_s64((int64x2_t)VEC_CARRY, 62);
    VEC_V6_V7_S6_S7 = vbicq_s32(VEC_V6_V7_S6_S7, VEC_CONST_CARRYS);

    
    VEC_BUFFER = vandq_s32(VEC_P8_0_P8_0 ,(int32x4_t)VEC_ADD_P_COND);
    VEC_V8_V9_S8_S9 = vaddq_s32(VEC_V8_V9_S8_S9, VEC_BUFFER);
    VEC_V8_V9_S8_S9 = vaddq_s32(VEC_V8_V9_S8_S9, VEC_CARRY);


    
    
    
    
    
    // reduction P once
    // V - P > 0 then reduction
    int64x2_t VEC_REDUCTION_COND = {0};
    VEC_BUFFER = vsubq_s32(VEC_V0_V1_S0_S1, VEC_P0_P1_P0_P1);
    VEC_BUFFER = vshrq_n_s32(VEC_BUFFER, 31);
    VEC_BUFFER = (int32x4_t)vshlq_n_s64((int64x2_t)VEC_BUFFER, 32);

    VEC_BUFFER = vsubq_s32(VEC_BUFFER, VEC_P0_P1_P0_P1);
    VEC_BUFFER = vaddq_s32(VEC_BUFFER, VEC_V0_V1_S0_S1);
    VEC_BUFFER = vshrq_n_s64((int64x2_t)VEC_BUFFER, 63);


    VEC_BUFFER = vsubq_s32(VEC_BUFFER, VEC_P2_P3_P2_P3);
    VEC_BUFFER = vaddq_s32(VEC_BUFFER, VEC_V2_V3_S2_S3);
    VEC_BUFFER = vshrq_n_s32(VEC_BUFFER, 31);
    VEC_BUFFER = (int32x4_t)vshlq_n_s64((int64x2_t)VEC_BUFFER, 32);

    VEC_BUFFER = vsubq_s32(VEC_BUFFER, VEC_P2_P3_P2_P3);
    VEC_BUFFER = vaddq_s32(VEC_BUFFER, VEC_V2_V3_S2_S3);
    VEC_BUFFER = vshrq_n_s64((int64x2_t)VEC_BUFFER, 63);


    VEC_BUFFER = vsubq_s32(VEC_BUFFER, VEC_P4_P5_P4_P5);
    VEC_BUFFER = vaddq_s32(VEC_BUFFER, VEC_V4_V5_S4_S5);
    VEC_BUFFER = vshrq_n_s32(VEC_BUFFER, 31);
    VEC_BUFFER = (int32x4_t)vshlq_n_s64((int64x2_t)VEC_BUFFER, 32);

    VEC_BUFFER = vsubq_s32(VEC_BUFFER, VEC_P4_P5_P4_P5);
    VEC_BUFFER = vaddq_s32(VEC_BUFFER, VEC_V4_V5_S4_S5);
    VEC_BUFFER = vshrq_n_s64((int64x2_t)VEC_BUFFER, 63);
    

    VEC_BUFFER = vsubq_s32(VEC_BUFFER, VEC_P6_P7_P6_P7);
    VEC_BUFFER = vaddq_s32(VEC_BUFFER, VEC_V6_V7_S6_S7);
    VEC_BUFFER = vshrq_n_s32(VEC_BUFFER, 31);
    VEC_BUFFER = (int32x4_t)vshlq_n_s64((int64x2_t)VEC_BUFFER, 32);

    VEC_BUFFER = vsubq_s32(VEC_BUFFER, VEC_P6_P7_P6_P7);
    VEC_BUFFER = vaddq_s32(VEC_BUFFER, VEC_V6_V7_S6_S7);
    VEC_BUFFER = vshrq_n_s64((int64x2_t)VEC_BUFFER, 63);


    VEC_BUFFER = vsubq_s32(VEC_BUFFER, VEC_P8_0_P8_0);
    VEC_BUFFER = vaddq_s32(VEC_BUFFER, VEC_V8_V9_S8_S9);
    VEC_BUFFER = vshlq_n_s64((int64x2_t)VEC_BUFFER, 32);
    VEC_REDUCTION_COND = vshrq_n_s64((int64x2_t)VEC_BUFFER, 63);

    VEC_REDUCTION_COND = (int64x2_t)vmvnq_s32((int32x4_t)VEC_REDUCTION_COND);


    // print_u32x4("VEC_REDUCTION_COND", (uint32x4_t)VEC_REDUCTION_COND);


    // VEC_BUFFER = vandq_s32(VEC_P0_P1_P0_P1 ,(int32x4_t)VEC_REDUCTION_COND);
    // VEC_V0_V1_S0_S1 = vsubq_s32(VEC_V0_V1_S0_S1, VEC_BUFFER);
    // VEC_CARRY = vbicq_s32(VEC_V0_V1_S0_S1, VEC_CONST_4X_2P30M1);
    // VEC_CARRY = (int32x4_t)vshlq_n_s64((int64x2_t)VEC_CARRY, 2);


    // VEC_V0_V1_S0_S1 = vaddq_s32(VEC_V0_V1_S0_S1, VEC_CARRY);
    // VEC_CARRY = vbicq_s32(VEC_V0_V1_S0_S1, VEC_CONST_4X_2P30M1);
    // VEC_CARRY = (int32x4_t)vshrq_n_s64((int64x2_t)VEC_CARRY, 62);
    // VEC_V0_V1_S0_S1 = vbicq_s32(VEC_V0_V1_S0_S1, VEC_CONST_CARRYS);


    // VEC_BUFFER = vandq_s32(VEC_P2_P3_P2_P3 ,(int32x4_t)VEC_ADD_P_COND);
    // VEC_V2_V3_S2_S3 = vaddq_s32(VEC_V2_V3_S2_S3, VEC_BUFFER);
    // VEC_V2_V3_S2_S3 = vaddq_s32(VEC_V2_V3_S2_S3, VEC_CARRY);
    // VEC_CARRY = vbicq_s32(VEC_V2_V3_S2_S3, VEC_CONST_4X_2P30M1);
    // VEC_CARRY = (int32x4_t)vshlq_n_s64((int64x2_t)VEC_CARRY, 2);
    // VEC_V2_V3_S2_S3 = vaddq_s32(VEC_V2_V3_S2_S3, VEC_CARRY);
    // VEC_CARRY = vbicq_s32(VEC_V2_V3_S2_S3, VEC_CONST_4X_2P30M1);
    // VEC_CARRY = (int32x4_t)vshrq_n_s64((int64x2_t)VEC_CARRY, 62);
    // VEC_V2_V3_S2_S3 = vbicq_s32(VEC_V2_V3_S2_S3, VEC_CONST_CARRYS);


    // VEC_BUFFER = vandq_s32(VEC_P4_P5_P4_P5 ,(int32x4_t)VEC_ADD_P_COND);
    // VEC_V4_V5_S4_S5 = vaddq_s32(VEC_V4_V5_S4_S5, VEC_BUFFER);
    // VEC_V4_V5_S4_S5 = vaddq_s32(VEC_V4_V5_S4_S5, VEC_CARRY);
    // VEC_CARRY = vbicq_s32(VEC_V4_V5_S4_S5, VEC_CONST_4X_2P30M1);
    // VEC_CARRY = (int32x4_t)vshlq_n_s64((int64x2_t)VEC_CARRY, 2);
    // VEC_V4_V5_S4_S5 = vaddq_s32(VEC_V4_V5_S4_S5, VEC_CARRY);
    // VEC_CARRY = vbicq_s32(VEC_V4_V5_S4_S5, VEC_CONST_4X_2P30M1);
    // VEC_CARRY = (int32x4_t)vshrq_n_s64((int64x2_t)VEC_CARRY, 62);
    // VEC_V4_V5_S4_S5 = vbicq_s32(VEC_V4_V5_S4_S5, VEC_CONST_CARRYS);


    // VEC_BUFFER = vandq_s32(VEC_P6_P7_P6_P7 ,(int32x4_t)VEC_ADD_P_COND);
    // VEC_V6_V7_S6_S7 = vaddq_s32(VEC_V6_V7_S6_S7, VEC_BUFFER);
    // VEC_V6_V7_S6_S7 = vaddq_s32(VEC_V6_V7_S6_S7, VEC_CARRY);
    // VEC_CARRY = vbicq_s32(VEC_V6_V7_S6_S7, VEC_CONST_4X_2P30M1);
    // VEC_CARRY = (int32x4_t)vshlq_n_s64((int64x2_t)VEC_CARRY, 2);
    // VEC_V6_V7_S6_S7 = vaddq_s32(VEC_V6_V7_S6_S7, VEC_CARRY);
    // VEC_CARRY = vbicq_s32(VEC_V6_V7_S6_S7, VEC_CONST_4X_2P30M1);
    // VEC_CARRY = (int32x4_t)vshrq_n_s64((int64x2_t)VEC_CARRY, 62);
    // VEC_V6_V7_S6_S7 = vbicq_s32(VEC_V6_V7_S6_S7, VEC_CONST_CARRYS);

    
    // VEC_BUFFER = vandq_s32(VEC_P8_0_P8_0 ,(int32x4_t)VEC_ADD_P_COND);
    // VEC_V8_V9_S8_S9 = vaddq_s32(VEC_V8_V9_S8_S9, VEC_BUFFER);
    // VEC_V8_V9_S8_S9 = vaddq_s32(VEC_V8_V9_S8_S9, VEC_CARRY);




    












    // print_u32x4("VEC_V0_V1_S0_S1", (uint32x4_t)VEC_V0_V1_S0_S1);
    // print_u32x4("VEC_V2_V3_S2_S3", (uint32x4_t)VEC_V2_V3_S2_S3);
    // print_u32x4("VEC_V4_V5_S4_S5", (uint32x4_t)VEC_V4_V5_S4_S5);
    // print_u32x4("VEC_V6_V7_S6_S7", (uint32x4_t)VEC_V6_V7_S6_S7);
    // print_u32x4("VEC_V8_V9_S8_S9", (uint32x4_t)VEC_V8_V9_S8_S9);
    // printf("-------------------------------------------------------------\n");



    vst1q_s32((int32_t *)&tmp[IDX_VEC_V0_V1_S0_S1], VEC_V0_V1_S0_S1);
    vst1q_s32((int32_t *)&tmp[IDX_VEC_V2_V3_S2_S3], VEC_V2_V3_S2_S3);
    vst1q_s32((int32_t *)&tmp[IDX_VEC_V4_V5_S4_S5], VEC_V4_V5_S4_S5);
    vst1q_s32((int32_t *)&tmp[IDX_VEC_V6_V7_S6_S7], VEC_V6_V7_S6_S7);
    vst1q_s32((int32_t *)&tmp[IDX_VEC_V8_V9_S8_S9], VEC_V8_V9_S8_S9);



}

