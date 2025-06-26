#include <stdint.h>
#include <stdio.h>
#include "../slot.h"
//#include <gmp.h>
#include <arm_neon.h>
#include <inttypes.h>
#include <assert.h>

static inline void print_u32x4(const char *tag, uint32x4_t v)
{
    uint32_t buf[4];          // 4×32-bit = 128-bit
    vst1q_u32(buf, v);        // 將向量存回一般記憶體
    printf("%s = 0x%08" PRIx32 " 0x%08" PRIx32
                  " 0x%08" PRIx32 " 0x%08" PRIx32 "\n",
           tag, buf[0], buf[1], buf[2], buf[3]);
}



void update_FG(uint64_t *tmp) {

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


    int32x4_t VEC_CONST_2X_2P30M1 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_CONST_2X_2P30M1]);
    int32x4_t VEC_CONST_2X_2P32M1 = (int32x4_t)vdupq_n_s64(((int64_t)1<<32) - 1);


    int32x4_t VEC_F0_F1_G0_G1 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_F0_F1_G0_G1]);
    int32x4_t VEC_F2_F3_G2_G3 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_F2_F3_G2_G3]);
    int32x4_t VEC_F4_F5_G4_G5 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_F4_F5_G4_G5]);
    int32x4_t VEC_F6_F7_G6_G7 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_F6_F7_G6_G7]);
    int32x4_t VEC_F8_F9_G8_G9 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_F8_F9_G8_G9]);    

    int32x4_t VEC_UU0_RR0_VV0_SS0 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_UU0_RR0_VV0_SS0]);
    int32x4_t VEC_UU1_RR1_VV1_SS1 = vld1q_s32((const int32_t *)&tmp[IDX_VEC_UU1_RR1_VV1_SS1]);


    int64x2_t VEC_PROD = {0};
    int32x4_t VEC_BUFFER = {0};

    // limb 0
    VEC_PROD = vmull_laneq_s32(vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_F0_F1_G0_G1, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_F0_F1_G0_G1, 2);

    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);

    // limb 1
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_F0_F1_G0_G1, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_F0_F1_G0_G1, 3);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_F0_F1_G0_G1, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_F0_F1_G0_G1, 2);

    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);

    // limb 2
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_F2_F3_G2_G3, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_F2_F3_G2_G3, 2);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_F0_F1_G0_G1, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_F0_F1_G0_G1, 3);

    VEC_F0_F1_G0_G1 = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);    

    // limb 3
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_F2_F3_G2_G3, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_F2_F3_G2_G3, 3);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_F2_F3_G2_G3, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_F2_F3_G2_G3, 2);

    VEC_BUFFER = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);
    VEC_BUFFER = vshlq_n_s64((int64x2_t)VEC_BUFFER, 32);
    VEC_F0_F1_G0_G1 = vorrq_s32(VEC_F0_F1_G0_G1, VEC_BUFFER);

    // limb 4
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_F4_F5_G4_G5, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_F4_F5_G4_G5, 2);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_F2_F3_G2_G3, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_F2_F3_G2_G3, 3);

    VEC_F2_F3_G2_G3 = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);

    
    // limb 5
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_F4_F5_G4_G5, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_F4_F5_G4_G5, 3);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_F4_F5_G4_G5, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_F4_F5_G4_G5, 2);
    
    VEC_BUFFER = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);
    VEC_BUFFER = vshlq_n_s64((int64x2_t)VEC_BUFFER, 32);
    VEC_F2_F3_G2_G3 = vorrq_s32(VEC_F2_F3_G2_G3, VEC_BUFFER);

    // limb 6
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_F6_F7_G6_G7, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_F6_F7_G6_G7, 2);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_F4_F5_G4_G5, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_F4_F5_G4_G5, 3);

    VEC_F4_F5_G4_G5 = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);

    // limb 7
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_F6_F7_G6_G7, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_F6_F7_G6_G7, 3);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_F6_F7_G6_G7, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_F6_F7_G6_G7, 2);
    
    
    VEC_BUFFER = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);
    VEC_BUFFER = vshlq_n_s64((int64x2_t)VEC_BUFFER, 32);
    VEC_F4_F5_G4_G5 = vorrq_s32(VEC_F4_F5_G4_G5, VEC_BUFFER);

    // limb 8
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU0_RR0_VV0_SS0), VEC_F8_F9_G8_G9, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU0_RR0_VV0_SS0), VEC_F8_F9_G8_G9, 2);    
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_F6_F7_G6_G7, 1);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_F6_F7_G6_G7, 3);

    VEC_F6_F7_G6_G7 = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);


    // limb 9
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_low_s32(VEC_UU1_RR1_VV1_SS1), VEC_F8_F9_G8_G9, 0);
    VEC_PROD = vmlal_laneq_s32(VEC_PROD, vget_high_s32(VEC_UU1_RR1_VV1_SS1), VEC_F8_F9_G8_G9, 2);

    VEC_BUFFER = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P30M1);
    VEC_PROD = vshrq_n_s64(VEC_PROD, 30);
    VEC_BUFFER = vshlq_n_s64((int64x2_t)VEC_BUFFER, 32);
    VEC_F6_F7_G6_G7 = vorrq_s32(VEC_F6_F7_G6_G7, VEC_BUFFER);

    VEC_F8_F9_G8_G9 = vandq_s32((int32x4_t)VEC_PROD, VEC_CONST_2X_2P32M1);



    print_u32x4("VEC_F0_F1_G0_G1", (uint32x4_t)VEC_F0_F1_G0_G1);
    print_u32x4("VEC_F2_F3_G2_G3", (uint32x4_t)VEC_F2_F3_G2_G3);
    print_u32x4("VEC_F4_F5_G4_G5", (uint32x4_t)VEC_F4_F5_G4_G5);
    print_u32x4("VEC_F6_F7_G6_G7", (uint32x4_t)VEC_F6_F7_G6_G7);
    print_u32x4("VEC_F8_F9_G8_G9", (uint32x4_t)VEC_F8_F9_G8_G9);
    printf("-------------------------------------------------------------\n");


    vst1q_s32((int32_t *)&tmp[IDX_VEC_F0_F1_G0_G1], VEC_F0_F1_G0_G1);
    vst1q_s32((int32_t *)&tmp[IDX_VEC_F2_F3_G2_G3], VEC_F2_F3_G2_G3);
    vst1q_s32((int32_t *)&tmp[IDX_VEC_F4_F5_G4_G5], VEC_F4_F5_G4_G5);
    vst1q_s32((int32_t *)&tmp[IDX_VEC_F6_F7_G6_G7], VEC_F6_F7_G6_G7);
    vst1q_s32((int32_t *)&tmp[IDX_VEC_F8_F9_G8_G9], VEC_F8_F9_G8_G9);

}