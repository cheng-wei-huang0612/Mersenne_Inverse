#include <stdint.h>
#include <stdio.h>
#include "slot.h"


void initialization(uint64_t *tmp) {
    // convert x into 9-limb radix 2^30 representation
    for(int i = 0; i < 9; i++) {
        tmp[IDX_G0 + i] = (uint32_t)(tmp[IDX_X0 + 0] & 0x3FFFFFFF);


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
    tmp[IDX_F0] = (uint32_t)(tmp[IDX_CONST_2P30M19]);
    for (size_t i = 1; i < 8; i++)
    {
        tmp[IDX_F0 + i] = (uint32_t)(tmp[IDX_CONST_2P30M1]);
    }
    tmp[IDX_F8] = (uint32_t)(tmp[IDX_CONST_2P15M1]);

    // initialize V <- 0 and S <- 1
    for (size_t i = 0; i < 9; i++)
    {
        tmp[IDX_V0 + i] = 0;
        tmp[IDX_S0 + i] = 0;
    }
    tmp[IDX_S0] = 1;

    // initialize delta
    tmp[IDX_DELTA] = 1;
}