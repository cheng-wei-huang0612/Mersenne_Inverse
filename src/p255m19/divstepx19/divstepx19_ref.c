#include <stdint.h>
#include "slot.h"

/* 介面改成整塊 tmp；用 slot 索引存取 */
void divstepx19(uint64_t *tmp)               /* ← 新原型 */
{
    int64_t FUV, GRS, DELTA;
    FUV = (tmp[IDX_f] & 0xFFFFF) - (tmp[IDX_CONST_2P41]);
    GRS = (tmp[IDX_g] & 0xFFFFF) - (tmp[IDX_CONST_2P62]);
    DELTA = tmp[IDX_DELTA];
    
    int64_t m1, ff;


    // int64_t *delta = (int64_t *)&tmp[IDX_DELTA];
    // int64_t *fuv   = (int64_t *)&tmp[IDX_FUV];
    // int64_t *grs   = (int64_t *)&tmp[IDX_GRS];

    for (int i = 0; i < 19; i++)
    {
        m1 = DELTA - 1;
        if ((GRS & 1) == 1) {
            ff = FUV;
        } else {
            ff = 0;
        }
        
        int64_t rotated_GRS = ((uint64_t)GRS << 63) | ((uint64_t)GRS >> 1);
        int64_t tst_result = m1 & rotated_GRS;

        // DELTA = m1 if N=0 else -DELTA
        if (tst_result >= 0) {  // N flag (negative bit) is clear
            DELTA = m1;
        } else {
            DELTA = -DELTA;
        }

        // FUV = GRS if N=1 else FUV
        if (tst_result < 0) {  // N flag is set
            FUV = GRS;
        }

        // ff = ff if N=0 else -ff
        if (tst_result < 0) {  // negative check for ff
            ff = -ff;
        }

        // GRS = (GRS + ff) >> 1 (算術位移)
        GRS = (GRS + ff) >> 1;




    }

    tmp[IDX_FUV] = FUV;
    tmp[IDX_GRS] = GRS;
    tmp[IDX_DELTA] = DELTA;
}