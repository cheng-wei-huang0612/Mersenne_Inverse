
# Generate by extraction_to_uvrs_1.py

enter extraction_to_uvrs
include "slot.h"
input input_x0

int64 FUV
int64 GRS
int64 CONST_2P20A2P41
int64 CONST_2P20


int64 u
int64 v
int64 r
int64 s



FUV = mem64[input_x0 + BYTE_IDX_FUV]
GRS = mem64[input_x0 + BYTE_IDX_GRS]

CONST_2P20A2P41 = mem64[input_x0 + BYTE_IDX_CONST_2P20A2P41]
CONST_2P20 = mem64[input_x0 + BYTE_IDX_CONST_2P20]

v = FUV + CONST_2P20A2P41
v = v signed>> 42

u = FUV + CONST_2P20
u = u << 22
u = u signed>> 43

s = GRS + CONST_2P20A2P41
s = s signed>> 42

r = GRS + CONST_2P20
r = r << 22
r = r signed>> 43


mem64[input_x0 + BYTE_IDX_u] = u
mem64[input_x0 + BYTE_IDX_v] = v
mem64[input_x0 + BYTE_IDX_r] = r
mem64[input_x0 + BYTE_IDX_s] = s


return
