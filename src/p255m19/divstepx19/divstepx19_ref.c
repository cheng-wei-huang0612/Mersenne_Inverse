#include <stdint.h>
#include "slot.h"

/* 介面改成整塊 tmp；用 slot 索引存取 */
void divstepx19(uint64_t *tmp)               /* ← 新原型 */
{
    tmp[IDX_FUV] = (tmp[IDX_f] & 0xFFFFF) - ( (int64_t) 1 << 41 );
    tmp[IDX_GRS] = (tmp[IDX_g] & 0xFFFFF) - ( (int64_t) 1 << 62 );

    int64_t *delta = (int64_t *)&tmp[IDX_DELTA];
    int64_t *fuv   = (int64_t *)&tmp[IDX_FUV];
    int64_t *grs   = (int64_t *)&tmp[IDX_GRS];

    for (int i = 0; i < 19; i++)
    {   
        int64_t g0_and_1 = (*grs) & 1;
        
        int64_t cond  = (~((*delta - 1) >> 63)) & g0_and_1;
        int64_t cmask = -cond;
        int64_t nmask = ~cmask;
        
        int64_t fuv_new = (nmask & *fuv) ^ (cmask & *grs);
        int64_t grs_new = (cmask & (-*fuv)) ^ (nmask & *grs);
        
        *fuv   = fuv_new;
        *grs   = grs_new;
        
        int64_t delta_swap = *delta ^ (-*delta);
        *delta ^= (cmask & delta_swap);
        
        *grs   = (((-g0_and_1) & *fuv) + *grs) >> 1;
        *delta += 2;
    }
}