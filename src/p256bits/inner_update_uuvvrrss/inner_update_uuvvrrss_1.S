
# qhasm: int64 input_x0

# qhasm: int64 input_x1

# qhasm: int64 input_x2

# qhasm: int64 input_x3

# qhasm: int64 input_x4

# qhasm: int64 input_x5

# qhasm: int64 input_x6

# qhasm: int64 input_x7

# qhasm: int64 output_x0

# qhasm: int64 calleesaved_x18

# qhasm: int64 calleesaved_x19

# qhasm: int64 calleesaved_x20

# qhasm: int64 calleesaved_x21

# qhasm: int64 calleesaved_x22

# qhasm: int64 calleesaved_x23

# qhasm: int64 calleesaved_x24

# qhasm: int64 calleesaved_x25

# qhasm: int64 calleesaved_x26

# qhasm: int64 calleesaved_x27

# qhasm: int64 calleesaved_x28

# qhasm: int64 calleesaved_x29

# qhasm: reg128 input_v0

# qhasm: reg128 input_v1

# qhasm: reg128 input_v2

# qhasm: reg128 input_v3

# qhasm: reg128 input_v4

# qhasm: reg128 input_v5

# qhasm: reg128 input_v6

# qhasm: reg128 input_v7

# qhasm: reg128 output_v0

# qhasm: reg128 calleesaved_v8

# qhasm: reg128 calleesaved_v9

# qhasm: reg128 calleesaved_v10

# qhasm: reg128 calleesaved_v11

# qhasm: reg128 calleesaved_v12

# qhasm: reg128 calleesaved_v13

# qhasm: reg128 calleesaved_v14

# qhasm: reg128 calleesaved_v15

# qhasm: include "slot.h"
#include "slot.h"

# qhasm: enter inner_update_uuvvrrss
.align 4
.global _inner_update_uuvvrrss
.global inner_update_uuvvrrss
_inner_update_uuvvrrss:
inner_update_uuvvrrss:

# qhasm: int64 pointer_tmp

# qhasm: input pointer_tmp

# qhasm: int64 u

# qhasm: int64 v

# qhasm: int64 r

# qhasm: int64 s

# qhasm: int64 UU

# qhasm: int64 VV

# qhasm: int64 RR

# qhasm: int64 SS

# qhasm: u = mem64[pointer_tmp + BYTE_IDX_u]
# asm 1: ldr >u=int64#2, [<pointer_tmp=int64#1, #BYTE_IDX_u]
# asm 2: ldr >u=x1, [<pointer_tmp=x0, #BYTE_IDX_u]
ldr x1, [x0, #BYTE_IDX_u]

# qhasm: v = mem64[pointer_tmp + BYTE_IDX_v]
# asm 1: ldr >v=int64#3, [<pointer_tmp=int64#1, #BYTE_IDX_v]
# asm 2: ldr >v=x2, [<pointer_tmp=x0, #BYTE_IDX_v]
ldr x2, [x0, #BYTE_IDX_v]

# qhasm: r = mem64[pointer_tmp + BYTE_IDX_r]
# asm 1: ldr >r=int64#4, [<pointer_tmp=int64#1, #BYTE_IDX_r]
# asm 2: ldr >r=x3, [<pointer_tmp=x0, #BYTE_IDX_r]
ldr x3, [x0, #BYTE_IDX_r]

# qhasm: s = mem64[pointer_tmp + BYTE_IDX_s]
# asm 1: ldr >s=int64#5, [<pointer_tmp=int64#1, #BYTE_IDX_s]
# asm 2: ldr >s=x4, [<pointer_tmp=x0, #BYTE_IDX_s]
ldr x4, [x0, #BYTE_IDX_s]

# qhasm: UU = mem64[pointer_tmp + BYTE_IDX_UU]
# asm 1: ldr >UU=int64#6, [<pointer_tmp=int64#1, #BYTE_IDX_UU]
# asm 2: ldr >UU=x5, [<pointer_tmp=x0, #BYTE_IDX_UU]
ldr x5, [x0, #BYTE_IDX_UU]

# qhasm: VV = mem64[pointer_tmp + BYTE_IDX_VV]
# asm 1: ldr >VV=int64#7, [<pointer_tmp=int64#1, #BYTE_IDX_VV]
# asm 2: ldr >VV=x6, [<pointer_tmp=x0, #BYTE_IDX_VV]
ldr x6, [x0, #BYTE_IDX_VV]

# qhasm: RR = mem64[pointer_tmp + BYTE_IDX_RR]
# asm 1: ldr >RR=int64#8, [<pointer_tmp=int64#1, #BYTE_IDX_RR]
# asm 2: ldr >RR=x7, [<pointer_tmp=x0, #BYTE_IDX_RR]
ldr x7, [x0, #BYTE_IDX_RR]

# qhasm: SS = mem64[pointer_tmp + BYTE_IDX_SS]
# asm 1: ldr >SS=int64#9, [<pointer_tmp=int64#1, #BYTE_IDX_SS]
# asm 2: ldr >SS=x8, [<pointer_tmp=x0, #BYTE_IDX_SS]
ldr x8, [x0, #BYTE_IDX_SS]

# qhasm: int64 buf

# qhasm: buf = u * UU
# asm 1: mul >buf=int64#10,<u=int64#2,<UU=int64#6
# asm 2: mul >buf=x9,<u=x1,<UU=x5
mul x9,x1,x5

# qhasm: buf = buf + v * RR
# asm 1: madd >buf=int64#10, <v=int64#3, <RR=int64#8, <buf=int64#10
# asm 2: madd >buf=x9, <v=x2, <RR=x7, <buf=x9
madd x9, x2, x7, x9

# qhasm: mem64[pointer_tmp + BYTE_IDX_UU] = buf
# asm 1: str <buf=int64#10, [<pointer_tmp=int64#1, #BYTE_IDX_UU]
# asm 2: str <buf=x9, [<pointer_tmp=x0, #BYTE_IDX_UU]
str x9, [x0, #BYTE_IDX_UU]

# qhasm: buf = u * VV
# asm 1: mul >buf=int64#2,<u=int64#2,<VV=int64#7
# asm 2: mul >buf=x1,<u=x1,<VV=x6
mul x1,x1,x6

# qhasm: buf = buf + v * SS
# asm 1: madd >buf=int64#2, <v=int64#3, <SS=int64#9, <buf=int64#2
# asm 2: madd >buf=x1, <v=x2, <SS=x8, <buf=x1
madd x1, x2, x8, x1

# qhasm: mem64[pointer_tmp + BYTE_IDX_VV] = buf
# asm 1: str <buf=int64#2, [<pointer_tmp=int64#1, #BYTE_IDX_VV]
# asm 2: str <buf=x1, [<pointer_tmp=x0, #BYTE_IDX_VV]
str x1, [x0, #BYTE_IDX_VV]

# qhasm: buf = r * UU
# asm 1: mul >buf=int64#2,<r=int64#4,<UU=int64#6
# asm 2: mul >buf=x1,<r=x3,<UU=x5
mul x1,x3,x5

# qhasm: buf = buf + s * RR
# asm 1: madd >buf=int64#2, <s=int64#5, <RR=int64#8, <buf=int64#2
# asm 2: madd >buf=x1, <s=x4, <RR=x7, <buf=x1
madd x1, x4, x7, x1

# qhasm: mem64[pointer_tmp + BYTE_IDX_RR] = buf
# asm 1: str <buf=int64#2, [<pointer_tmp=int64#1, #BYTE_IDX_RR]
# asm 2: str <buf=x1, [<pointer_tmp=x0, #BYTE_IDX_RR]
str x1, [x0, #BYTE_IDX_RR]

# qhasm: buf = r * VV
# asm 1: mul >buf=int64#2,<r=int64#4,<VV=int64#7
# asm 2: mul >buf=x1,<r=x3,<VV=x6
mul x1,x3,x6

# qhasm: buf = buf + s * SS
# asm 1: madd >buf=int64#2, <s=int64#5, <SS=int64#9, <buf=int64#2
# asm 2: madd >buf=x1, <s=x4, <SS=x8, <buf=x1
madd x1, x4, x8, x1

# qhasm: mem64[pointer_tmp + BYTE_IDX_SS] = buf
# asm 1: str <buf=int64#2, [<pointer_tmp=int64#1, #BYTE_IDX_SS]
# asm 2: str <buf=x1, [<pointer_tmp=x0, #BYTE_IDX_SS]
str x1, [x0, #BYTE_IDX_SS]

# qhasm: return
ret
