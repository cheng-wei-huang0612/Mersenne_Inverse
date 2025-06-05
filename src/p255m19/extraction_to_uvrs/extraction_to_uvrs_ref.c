#include <stdint.h>
#include <stdio.h>
#include "slot.h"


void extraction_to_uvrs(uint64_t *tmp) {
    int64_t *fuv = (int64_t *)&tmp[IDX_FUV];
    int64_t *grs = (int64_t *)&tmp[IDX_GRS];
    uint64_t *uvrs = (uint64_t *)&tmp[IDX_u];
    int64_t u, v, r, s;

    v = ((*fuv) + ((int64_t)1 << 20) + ((int64_t)1 << 41));
    v = (v) >> 42;

    u = ((*fuv) + ((int64_t)1 << 20));
    u = (uint64_t) (u) << 22;
    u = (u) >> 43;



    s = ((*grs) + ((int64_t)1 << 20) + ((int64_t)1 << 41));
    s = (s) >> 42;

    r = ((*grs) + ((int64_t)1 << 20));
    r = (uint64_t) (r) << 22;
    r = (r) >> 43;

    uvrs[0] = u;
    uvrs[1] = v;
    uvrs[2] = r;
    uvrs[3] = s;
}


