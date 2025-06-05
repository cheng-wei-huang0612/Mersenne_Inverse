#include <stdint.h>
#include <stdio.h>
#include "slot.h"

#include <gmp.h>
void inner_update_uuvvrrss(uint64_t *tmp) {

    int64_t *uuvvrrss = (int64_t *)&tmp[IDX_UU];
    int64_t *uvrs = (int64_t *)&tmp[IDX_u];

    int64_t u = uvrs[0];
    int64_t v = uvrs[1];
    int64_t r = uvrs[2];
    int64_t s = uvrs[3];

    int64_t uu = uuvvrrss[0];
    int64_t vv = uuvvrrss[1];
    int64_t rr = uuvvrrss[2];
    int64_t ss = uuvvrrss[3];

    int64_t uu_new = (u) * (uu) + (v) * (rr);         
    int64_t rr_new = (r) * (uu) + (s) * (rr);
    int64_t vv_new = (u) * (vv) + (v) * (ss);
    int64_t ss_new = (r) * (vv) + (s) * (ss);

    uu = uu_new;
    rr = rr_new;
    vv = vv_new;
    ss = ss_new;

    uvrs[0] = u;
    uvrs[1] = v;
    uvrs[2] = r;
    uvrs[3] = s;

    uuvvrrss[0] = uu;
    uuvvrrss[1] = vv;
    uuvvrrss[2] = rr;
    uuvvrrss[3] = ss;
}




