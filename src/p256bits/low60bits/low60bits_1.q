
# Generate by low60bits_1.py

include "slot.h"
enter low60bits

int64 pointer_tmp
input pointer_tmp

int64 F0F1
int64 G0G1
reg128 VEC_F0_F1_G0_G1
reg128 VEC_CONST_2X_2P30M1
reg128 VEC_F0_G0
reg128 VEC_F1_G1
reg128 VEC_f_g
int64 f
int64 g

VEC_F0_F1_G0_G1 = mem128[pointer_tmp + BYTE_IDX_VEC_F0_F1_G0_G1]
VEC_CONST_2X_2P30M1 = mem128[pointer_tmp + BYTE_IDX_VEC_CONST_2X_2P30M1]

VEC_F0_G0 = VEC_F0_F1_G0_G1 & VEC_CONST_2X_2P30M1
2x VEC_F1_G1 = VEC_F0_F1_G0_G1 unsigned>> 32

2x VEC_f_g = VEC_F1_G1 << 30
VEC_f_g = VEC_f_g | VEC_F0_G0

f = VEC_f_g[0/2]
g = VEC_f_g[1/2]


mem64[pointer_tmp + BYTE_IDX_f] = f
mem64[pointer_tmp + BYTE_IDX_g] = g



return
