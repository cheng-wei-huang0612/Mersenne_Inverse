#include <stdint.h>
#include <stdio.h>
#include "slot.h"


void extraction_to_uuvvrrss(uint64_t *tmp) {
    int64_t *fuv = (int64_t *)&tmp[IDX_FUV];
    int64_t *grs = (int64_t *)&tmp[IDX_GRS];
    uint64_t *uuvvrrss = (uint64_t *)&tmp[IDX_UU];
    int64_t uu, vv, rr, ss;

    vv = ((*fuv) + ((int64_t)1 << 20) + ((int64_t)1 << 41));
    vv = (vv) >> 42;

    uu = ((*fuv) + ((int64_t)1 << 20));
    uu = (uint64_t) (uu) << 22;
    uu = (uu) >> 43;



    ss = ((*grs) + ((int64_t)1 << 20) + ((int64_t)1 << 41));
    ss = (ss) >> 42;

    rr = ((*grs) + ((int64_t)1 << 20));
    rr = (uint64_t) (rr) << 22;
    rr = (rr) >> 43;

    uuvvrrss[0] = uu;
    uuvvrrss[1] = vv;
    uuvvrrss[2] = rr;
    uuvvrrss[3] = ss;
}


