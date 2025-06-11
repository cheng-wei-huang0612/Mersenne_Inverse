
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

# qhasm: enter low60bits
.align 4
.global _low60bits
.global low60bits
_low60bits:
low60bits:

# qhasm: int64 pointer_tmp

# qhasm: input pointer_tmp

# qhasm: int64 F0F1

# qhasm: int64 G0G1

# qhasm: reg128 VEC_F0_F1_G0_G1

# qhasm: reg128 VEC_CONST_2X_2P30M1

# qhasm: reg128 VEC_F0_G0

# qhasm: reg128 VEC_F1_G1

# qhasm: reg128 VEC_f_g

# qhasm: int64 f

# qhasm: int64 g

# qhasm: VEC_F0_F1_G0_G1 = mem128[pointer_tmp + BYTE_IDX_VEC_F0_F1_G0_G1]
# asm 1: ldr >VEC_F0_F1_G0_G1=reg128#1%qregname,[<pointer_tmp=int64#1,#BYTE_IDX_VEC_F0_F1_G0_G1]
# asm 2: ldr >VEC_F0_F1_G0_G1=q0,[<pointer_tmp=x0,#BYTE_IDX_VEC_F0_F1_G0_G1]
ldr q0,[x0,#BYTE_IDX_VEC_F0_F1_G0_G1]

# qhasm: VEC_CONST_2X_2P30M1 = mem128[pointer_tmp + BYTE_IDX_VEC_CONST_2X_2P30M1]
# asm 1: ldr >VEC_CONST_2X_2P30M1=reg128#2%qregname,[<pointer_tmp=int64#1,#BYTE_IDX_VEC_CONST_2X_2P30M1]
# asm 2: ldr >VEC_CONST_2X_2P30M1=q1,[<pointer_tmp=x0,#BYTE_IDX_VEC_CONST_2X_2P30M1]
ldr q1,[x0,#BYTE_IDX_VEC_CONST_2X_2P30M1]

# qhasm: VEC_F0_G0 = VEC_F0_F1_G0_G1 & VEC_CONST_2X_2P30M1
# asm 1: and >VEC_F0_G0=reg128#2.16b, <VEC_F0_F1_G0_G1=reg128#1.16b, <VEC_CONST_2X_2P30M1=reg128#2.16b
# asm 2: and >VEC_F0_G0=v1.16b, <VEC_F0_F1_G0_G1=v0.16b, <VEC_CONST_2X_2P30M1=v1.16b
and v1.16b, v0.16b, v1.16b

# qhasm: 2x VEC_F1_G1 = VEC_F0_F1_G0_G1 unsigned>> 32
# asm 1: ushr >VEC_F1_G1=reg128#1.2d, <VEC_F0_F1_G0_G1=reg128#1.2d, #32
# asm 2: ushr >VEC_F1_G1=v0.2d, <VEC_F0_F1_G0_G1=v0.2d, #32
ushr v0.2d, v0.2d, #32

# qhasm: 2x VEC_f_g = VEC_F1_G1 << 30
# asm 1: shl >VEC_f_g=reg128#1.2d, <VEC_F1_G1=reg128#1.2d, #30
# asm 2: shl >VEC_f_g=v0.2d, <VEC_F1_G1=v0.2d, #30
shl v0.2d, v0.2d, #30

# qhasm: VEC_f_g = VEC_f_g | VEC_F0_G0
# asm 1: orr >VEC_f_g=reg128#1.16b, <VEC_f_g=reg128#1.16b, <VEC_F0_G0=reg128#2.16b
# asm 2: orr >VEC_f_g=v0.16b, <VEC_f_g=v0.16b, <VEC_F0_G0=v1.16b
orr v0.16b, v0.16b, v1.16b

# qhasm: f = VEC_f_g[0/2]
# asm 1: umov >f=int64#2, <VEC_f_g=reg128#1.d[0]
# asm 2: umov >f=x1, <VEC_f_g=v0.d[0]
umov x1, v0.d[0]

# qhasm: g = VEC_f_g[1/2]
# asm 1: umov >g=int64#3, <VEC_f_g=reg128#1.d[1]
# asm 2: umov >g=x2, <VEC_f_g=v0.d[1]
umov x2, v0.d[1]

# qhasm: mem64[pointer_tmp + BYTE_IDX_f] = f
# asm 1: str <f=int64#2, [<pointer_tmp=int64#1, #BYTE_IDX_f]
# asm 2: str <f=x1, [<pointer_tmp=x0, #BYTE_IDX_f]
str x1, [x0, #BYTE_IDX_f]

# qhasm: mem64[pointer_tmp + BYTE_IDX_g] = g
# asm 1: str <g=int64#3, [<pointer_tmp=int64#1, #BYTE_IDX_g]
# asm 2: str <g=x2, [<pointer_tmp=x0, #BYTE_IDX_g]
str x2, [x0, #BYTE_IDX_g]

# qhasm: return
ret
