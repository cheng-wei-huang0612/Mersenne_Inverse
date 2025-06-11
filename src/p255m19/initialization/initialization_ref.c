#include <stdint.h>
#include <stdio.h>
#include "slot.h"
#include <arm_neon.h>

void initialization(uint64_t *tmp) {
    // convert x into 9-limb radix 2^30 representation
    for (int i = 0; i < 9; i++) {
        tmp[IDX_VEC_G0 + i] = (uint32_t)(tmp[IDX_X0 + 0] & 0x3FFFFFFF);


        uint64_t t0 = tmp[IDX_X0 + 0];
        uint64_t t1 = tmp[IDX_X0 + 1];
        uint64_t t2 = tmp[IDX_X0 + 2];
        uint64_t t3 = tmp[IDX_X0 + 3];

        tmp[IDX_X0 + 0] = (t0 >> 30) | (t1 << 34);
        tmp[IDX_X0 + 1] = (t1 >> 30) | (t2 << 34);
        tmp[IDX_X0 + 2] = (t2 >> 30) | (t3 << 34);
        tmp[IDX_X0 + 3] =  t3 >> 30;
    }

    // initialize F <- P
    uint64_t buf;

    buf = (uint64_t)(tmp[IDX_CONST_2P30M19]) | 
          ((uint64_t)(tmp[IDX_CONST_2P30M1]) << 32);
    tmp[IDX_VEC_F0_F1_G0_G1] = buf;
    buf = (uint64_t)(tmp[IDX_VEC_G0]) | 
          ((uint64_t)(tmp[IDX_VEC_G1]) << 32);
    tmp[IDX_VEC_F0_F1_G0_G1_HI1] = buf;


    buf = (uint64_t)(tmp[IDX_CONST_2P30M1]) | 
          ((uint64_t)(tmp[IDX_CONST_2P30M1]) << 32);
    tmp[IDX_VEC_F2_F3_G2_G3] = buf;
    buf = (uint64_t)(tmp[IDX_VEC_G2]) | 
          ((uint64_t)(tmp[IDX_VEC_G3]) << 32);
    tmp[IDX_VEC_F2_F3_G2_G3_HI1] = buf;


    buf = (uint64_t)(tmp[IDX_CONST_2P30M1]) | 
          ((uint64_t)(tmp[IDX_CONST_2P30M1]) << 32);
    tmp[IDX_VEC_F4_F5_G4_G5] = buf;
    buf = (uint64_t)(tmp[IDX_VEC_G4]) | 
          ((uint64_t)(tmp[IDX_VEC_G5]) << 32);
    tmp[IDX_VEC_F4_F5_G4_G5_HI1] = buf;

    buf = (uint64_t)(tmp[IDX_CONST_2P30M1]) | 
          ((uint64_t)(tmp[IDX_CONST_2P30M1]) << 32);
    tmp[IDX_VEC_F6_F7_G6_G7] = buf;
    buf = (uint64_t)(tmp[IDX_VEC_G6]) | 
          ((uint64_t)(tmp[IDX_VEC_G7]) << 32);
    tmp[IDX_VEC_F6_F7_G6_G7_HI1] = buf;

    tmp[IDX_VEC_F8_F9_G8_G9] = (uint32_t)(tmp[IDX_CONST_2P15M1]);
    tmp[IDX_VEC_F8_F9_G8_G9_HI1] = (uint32_t)(tmp[IDX_VEC_G8]) ;

    // initialize V <- 0 and S <- 1
    tmp[IDX_VEC_V0_V1_S0_S1] = 0;
    tmp[IDX_VEC_V0_V1_S0_S1_HI1] = 0;
    tmp[IDX_VEC_V2_V3_S2_S3] = 0;
    tmp[IDX_VEC_V2_V3_S2_S3_HI1] = 0;
    tmp[IDX_VEC_V4_V5_S4_S5] = 0;
    tmp[IDX_VEC_V4_V5_S4_S5_HI1] = 0;
    tmp[IDX_VEC_V6_V7_S6_S7] = 0;
    tmp[IDX_VEC_V6_V7_S6_S7_HI1] = 0;
    tmp[IDX_VEC_V8_V9_S8_S9] = 0;
    tmp[IDX_VEC_V8_V9_S8_S9_HI1] = 0;


    tmp[IDX_VEC_V0_V1_S0_S1_HI1] = 1;

    // initialize delta
    tmp[IDX_DELTA] = 0;
}