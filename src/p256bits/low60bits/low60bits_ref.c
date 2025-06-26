#include <stdint.h>
#include <stdio.h>
#include <arm_neon.h>
#include "../slot.h"


void low60bits(uint64_t *tmp) {

    
    uint64x2_t VEC_F0_F1_G0_G1, VEC_F0_G0, VEC_F1_G1;
    uint64x2_t VEC_f_g;
    uint64x2_t VEC_CONST_2X_2P30M1;

    VEC_F0_F1_G0_G1 = vld1q_u64(&tmp[IDX_VEC_F0_F1_G0_G1]);
    VEC_CONST_2X_2P30M1 = vld1q_u64(&tmp[IDX_VEC_CONST_2X_2P30M1]);

    VEC_F0_G0 = vandq_u64(VEC_F0_F1_G0_G1, VEC_CONST_2X_2P30M1);

    VEC_F1_G1 = vshrq_n_u64(VEC_F0_F1_G0_G1, 32);
    VEC_f_g = vorrq_u64(vshlq_n_u64(VEC_F1_G1, 30), VEC_F0_G0);

    uint64_t f = vgetq_lane_u64(VEC_f_g, 0);
    uint64_t g = vgetq_lane_u64(VEC_f_g, 1);

    tmp[IDX_f] = f;
    tmp[IDX_g] = g;

}