#include <stdint.h>
#include <stdio.h>
#include "slot.h"


void extraction_to_uuvvrrss(uint64_t *tmp) {

    int64_t FUV, GRS;
    int64_t uu, vv, rr, ss;
    FUV = tmp[IDX_FUV];
    GRS = tmp[IDX_GRS];

    vv = (FUV + tmp[IDX_CONST_2P20A2P41]);
    tmp[IDX_VV] = (vv) >> 42;

    uu = (FUV + tmp[IDX_CONST_2P20]);
    uu = (uint64_t)(uu) << 22;
    tmp[IDX_UU] = (uu) >> 43;


    ss = (GRS + tmp[IDX_CONST_2P20A2P41]);
    tmp[IDX_SS] = (ss) >> 42;

    rr = (GRS + tmp[IDX_CONST_2P20]);
    rr = (uint64_t)(rr) << 22;
    tmp[IDX_RR] = (rr) >> 43;



}


