#include <stdint.h>
#include "slot.h"


void low60bits(uint64_t *tmp) {
    tmp[IDX_f] = (int64_t)(((tmp[IDX_F1] & 0x3FFFFFFF) << 30) | (tmp[IDX_F0] & 0x3FFFFFFF));
    tmp[IDX_g] = (int64_t)(((tmp[IDX_G1] & 0x3FFFFFFF) << 30) | (tmp[IDX_G0] & 0x3FFFFFFF));
}