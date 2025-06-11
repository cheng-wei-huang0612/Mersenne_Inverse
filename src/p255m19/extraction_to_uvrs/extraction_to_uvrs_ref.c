#include <stdint.h>
#include <stdio.h>
#include "slot.h"


void extraction_to_uvrs(uint64_t *tmp) {

    int64_t FUV, GRS;
    int64_t u, v, r, s;
    FUV = tmp[IDX_FUV];
    GRS = tmp[IDX_GRS];

    v = (FUV + tmp[IDX_CONST_2P20A2P41]);
    tmp[IDX_v] = (v) >> 42;

    u = (FUV + tmp[IDX_CONST_2P20]);
    u = (uint64_t)(u) << 22;
    tmp[IDX_u] = (u) >> 43;


    s = (GRS + tmp[IDX_CONST_2P20A2P41]);
    tmp[IDX_s] = (s) >> 42;

    r = (GRS + tmp[IDX_CONST_2P20]);
    r = (uint64_t)(r) << 22;
    tmp[IDX_r] = (r) >> 43;


}


