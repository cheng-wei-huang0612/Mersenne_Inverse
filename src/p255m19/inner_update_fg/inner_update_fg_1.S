
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

# qhasm: enter inner_update_fg
.align 4
.global _inner_update_fg
.global inner_update_fg
_inner_update_fg:
inner_update_fg:

# qhasm: input input_x0

# qhasm: int64 u

# qhasm: int64 v

# qhasm: int64 r

# qhasm: int64 s

# qhasm: int64 f

# qhasm: int64 f_lo

# qhasm: int64 f_hi

# qhasm: int64 f_new

# qhasm: int64 g

# qhasm: int64 g_lo

# qhasm: int64 g_hi

# qhasm: int64 g_new

# qhasm: f = mem64[input_x0 + BYTE_IDX_f]
# asm 1: ldr >f=int64#2, [<input_x0=int64#1, #BYTE_IDX_f]
# asm 2: ldr >f=x1, [<input_x0=x0, #BYTE_IDX_f]
ldr x1, [x0, #BYTE_IDX_f]

# qhasm: g = mem64[input_x0 + BYTE_IDX_g]
# asm 1: ldr >g=int64#3, [<input_x0=int64#1, #BYTE_IDX_g]
# asm 2: ldr >g=x2, [<input_x0=x0, #BYTE_IDX_g]
ldr x2, [x0, #BYTE_IDX_g]

# qhasm: u = mem64[input_x0 + BYTE_IDX_u]
# asm 1: ldr >u=int64#4, [<input_x0=int64#1, #BYTE_IDX_u]
# asm 2: ldr >u=x3, [<input_x0=x0, #BYTE_IDX_u]
ldr x3, [x0, #BYTE_IDX_u]

# qhasm: v = mem64[input_x0 + BYTE_IDX_v]
# asm 1: ldr >v=int64#5, [<input_x0=int64#1, #BYTE_IDX_v]
# asm 2: ldr >v=x4, [<input_x0=x0, #BYTE_IDX_v]
ldr x4, [x0, #BYTE_IDX_v]

# qhasm: r = mem64[input_x0 + BYTE_IDX_r]
# asm 1: ldr >r=int64#6, [<input_x0=int64#1, #BYTE_IDX_r]
# asm 2: ldr >r=x5, [<input_x0=x0, #BYTE_IDX_r]
ldr x5, [x0, #BYTE_IDX_r]

# qhasm: s = mem64[input_x0 + BYTE_IDX_s]
# asm 1: ldr >s=int64#7, [<input_x0=int64#1, #BYTE_IDX_s]
# asm 2: ldr >s=x6, [<input_x0=x0, #BYTE_IDX_s]
ldr x6, [x0, #BYTE_IDX_s]

# qhasm: f_lo = f & 0xFFFFFFFF
# asm 1: and >f_lo=int64#8, <f=int64#2, #0xFFFFFFFF
# asm 2: and >f_lo=x7, <f=x1, #0xFFFFFFFF
and x7, x1, #0xFFFFFFFF

# qhasm: f_hi = f signed>> 32
# asm 1: asr >f_hi=int64#2, <f=int64#2, #32
# asm 2: asr >f_hi=x1, <f=x1, #32
asr x1, x1, #32

# qhasm: g_lo = g & 0xFFFFFFFF
# asm 1: and >g_lo=int64#9, <g=int64#3, #0xFFFFFFFF
# asm 2: and >g_lo=x8, <g=x2, #0xFFFFFFFF
and x8, x2, #0xFFFFFFFF

# qhasm: g_hi = g signed>> 32
# asm 1: asr >g_hi=int64#3, <g=int64#3, #32
# asm 2: asr >g_hi=x2, <g=x2, #32
asr x2, x2, #32

# qhasm: int64 buf0

# qhasm: int64 buf1

# qhasm: buf0 = u * f_lo
# asm 1: mul >buf0=int64#10,<u=int64#4,<f_lo=int64#8
# asm 2: mul >buf0=x9,<u=x3,<f_lo=x7
mul x9,x3,x7

# qhasm: buf0 = v * g_lo + buf0
# asm 1: madd >buf0=int64#10, <v=int64#5, <g_lo=int64#9, <buf0=int64#10
# asm 2: madd >buf0=x9, <v=x4, <g_lo=x8, <buf0=x9
madd x9, x4, x8, x9

# qhasm: buf0 = buf0 signed>> 20
# asm 1: asr >buf0=int64#10, <buf0=int64#10, #20
# asm 2: asr >buf0=x9, <buf0=x9, #20
asr x9, x9, #20

# qhasm: buf1 = u * f_hi
# asm 1: mul >buf1=int64#4,<u=int64#4,<f_hi=int64#2
# asm 2: mul >buf1=x3,<u=x3,<f_hi=x1
mul x3,x3,x1

# qhasm: buf1 = v * g_hi + buf1
# asm 1: madd >buf1=int64#4, <v=int64#5, <g_hi=int64#3, <buf1=int64#4
# asm 2: madd >buf1=x3, <v=x4, <g_hi=x2, <buf1=x3
madd x3, x4, x2, x3

# qhasm: buf1 = buf1 << 12
# asm 1: lsl >buf1=int64#4, <buf1=int64#4, #12
# asm 2: lsl >buf1=x3, <buf1=x3, #12
lsl x3, x3, #12

# qhasm: f_new = buf0 + buf1
# asm 1: add >f_new=int64#4,<buf0=int64#10,<buf1=int64#4
# asm 2: add >f_new=x3,<buf0=x9,<buf1=x3
add x3,x9,x3

# qhasm: buf0 = r * f_lo
# asm 1: mul >buf0=int64#5,<r=int64#6,<f_lo=int64#8
# asm 2: mul >buf0=x4,<r=x5,<f_lo=x7
mul x4,x5,x7

# qhasm: buf0 = s * g_lo + buf0
# asm 1: madd >buf0=int64#5, <s=int64#7, <g_lo=int64#9, <buf0=int64#5
# asm 2: madd >buf0=x4, <s=x6, <g_lo=x8, <buf0=x4
madd x4, x6, x8, x4

# qhasm: buf0 = buf0 signed>> 20
# asm 1: asr >buf0=int64#5, <buf0=int64#5, #20
# asm 2: asr >buf0=x4, <buf0=x4, #20
asr x4, x4, #20

# qhasm: buf1 = r * f_hi
# asm 1: mul >buf1=int64#2,<r=int64#6,<f_hi=int64#2
# asm 2: mul >buf1=x1,<r=x5,<f_hi=x1
mul x1,x5,x1

# qhasm: buf1 = s * g_hi + buf1
# asm 1: madd >buf1=int64#2, <s=int64#7, <g_hi=int64#3, <buf1=int64#2
# asm 2: madd >buf1=x1, <s=x6, <g_hi=x2, <buf1=x1
madd x1, x6, x2, x1

# qhasm: buf1 = buf1 << 12
# asm 1: lsl >buf1=int64#2, <buf1=int64#2, #12
# asm 2: lsl >buf1=x1, <buf1=x1, #12
lsl x1, x1, #12

# qhasm: g_new = buf0 + buf1
# asm 1: add >g_new=int64#2,<buf0=int64#5,<buf1=int64#2
# asm 2: add >g_new=x1,<buf0=x4,<buf1=x1
add x1,x4,x1

# qhasm: mem64[input_x0 + BYTE_IDX_f] = f_new
# asm 1: str <f_new=int64#4, [<input_x0=int64#1, #BYTE_IDX_f]
# asm 2: str <f_new=x3, [<input_x0=x0, #BYTE_IDX_f]
str x3, [x0, #BYTE_IDX_f]

# qhasm: mem64[input_x0 + BYTE_IDX_g] = g_new
# asm 1: str <g_new=int64#2, [<input_x0=int64#1, #BYTE_IDX_g]
# asm 2: str <g_new=x1, [<input_x0=x0, #BYTE_IDX_g]
str x1, [x0, #BYTE_IDX_g]

# qhasm: return
ret
