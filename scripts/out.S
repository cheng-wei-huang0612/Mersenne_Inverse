
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

# qhasm: int64 CONST_2P20

# qhasm: int64 CONST_2P20A2P41

# qhasm: int64 CONST_2P41

# qhasm: int64 CONST_2P62

# qhasm: int64 DELTA

# qhasm: int64 FUV

# qhasm: int64 GRS

# qhasm: int64 f

# qhasm: int64 ff

# qhasm: int64 g

# qhasm: int64 m1

# qhasm: int64 r

# qhasm: int64 s

# qhasm: int64 u

# qhasm: int64 v

# qhasm: enter result
.align 4
.global _result
.global result
_result:
result:

# qhasm: CONST_2P41 = mem64[input_x0 + BYTE_IDX_CONST_2P41]
# asm 1: ldr >CONST_2P41=int64#2, [<input_x0=int64#1, #BYTE_IDX_CONST_2P41]
# asm 2: ldr >CONST_2P41=x1, [<input_x0=x0, #BYTE_IDX_CONST_2P41]
ldr x1, [x0, #BYTE_IDX_CONST_2P41]

# qhasm: CONST_2P62 = mem64[input_x0 + BYTE_IDX_CONST_2P62]
# asm 1: ldr >CONST_2P62=int64#3, [<input_x0=int64#1, #BYTE_IDX_CONST_2P62]
# asm 2: ldr >CONST_2P62=x2, [<input_x0=x0, #BYTE_IDX_CONST_2P62]
ldr x2, [x0, #BYTE_IDX_CONST_2P62]

# qhasm: f = mem64[input_x0 + BYTE_IDX_f]
# asm 1: ldr >f=int64#4, [<input_x0=int64#1, #BYTE_IDX_f]
# asm 2: ldr >f=x3, [<input_x0=x0, #BYTE_IDX_f]
ldr x3, [x0, #BYTE_IDX_f]

# qhasm: g = mem64[input_x0 + BYTE_IDX_g]
# asm 1: ldr >g=int64#5, [<input_x0=int64#1, #BYTE_IDX_g]
# asm 2: ldr >g=x4, [<input_x0=x0, #BYTE_IDX_g]
ldr x4, [x0, #BYTE_IDX_g]

# qhasm: DELTA = mem64[input_x0 + BYTE_IDX_DELTA]
# asm 1: ldr >DELTA=int64#6, [<input_x0=int64#1, #BYTE_IDX_DELTA]
# asm 2: ldr >DELTA=x5, [<input_x0=x0, #BYTE_IDX_DELTA]
ldr x5, [x0, #BYTE_IDX_DELTA]

# qhasm: f = f & 0xFFFFF
# asm 1: and >f=int64#4, <f=int64#4, #0xFFFFF
# asm 2: and >f=x3, <f=x3, #0xFFFFF
and x3, x3, #0xFFFFF

# qhasm: g = g & 0xFFFFF
# asm 1: and >g=int64#5, <g=int64#5, #0xFFFFF
# asm 2: and >g=x4, <g=x4, #0xFFFFF
and x4, x4, #0xFFFFF

# qhasm: FUV = f - CONST_2P41
# asm 1: sub >FUV=int64#2,<f=int64#4,<CONST_2P41=int64#2
# asm 2: sub >FUV=x1,<f=x3,<CONST_2P41=x1
sub x1,x3,x1

# qhasm: GRS = g - CONST_2P62
# asm 1: sub >GRS=int64#3,<g=int64#5,<CONST_2P62=int64#3
# asm 2: sub >GRS=x2,<g=x4,<CONST_2P62=x2
sub x2,x4,x2

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm:     m1 = DELTA - 1 
# asm 1: sub >m1=int64#4,<DELTA=int64#6,#1
# asm 2: sub >m1=x3,<DELTA=x5,#1
sub x3,x5,#1

# qhasm:     GRS & 1
# asm 1: tst <GRS=int64#3, #1
# asm 2: tst <GRS=x2, #1
tst x2, #1

# qhasm:     ff = FUV if Z=0 else 0
# asm 1: csel >ff=int64#5, <FUV=int64#2, xzr, ne
# asm 2: csel >ff=x4, <FUV=x1, xzr, ne
csel x4, x1, xzr, ne

# qhasm:     m1 & (GRS >>> 1)
# asm 1: tst <m1=int64#4, <GRS=int64#3, ROR #1
# asm 2: tst <m1=x3, <GRS=x2, ROR #1
tst x3, x2, ROR #1

# qhasm:     DELTA = m1 if N=0 else -DELTA
# asm 1: csneg >DELTA=int64#6, <m1=int64#4, <DELTA=int64#6, pl
# asm 2: csneg >DELTA=x5, <m1=x3, <DELTA=x5, pl
csneg x5, x3, x5, pl

# qhasm:     FUV = GRS if N=1 else FUV
# asm 1: csel >FUV=int64#2, <GRS=int64#3, <FUV=int64#2, mi
# asm 2: csel >FUV=x1, <GRS=x2, <FUV=x1, mi
csel x1, x2, x1, mi

# qhasm:     ff = ff if N=0 else -ff
# asm 1: csneg >ff=int64#5, <ff=int64#5, <ff=int64#5, pl
# asm 2: csneg >ff=x4, <ff=x4, <ff=x4, pl
csneg x4, x4, x4, pl

# qhasm:     GRS = GRS + ff
# asm 1: add >GRS=int64#3,<GRS=int64#3,<ff=int64#5
# asm 2: add >GRS=x2,<GRS=x2,<ff=x4
add x2,x2,x4

# qhasm:     GRS = GRS signed>> 1
# asm 1: asr >GRS=int64#3, <GRS=int64#3, #1
# asm 2: asr >GRS=x2, <GRS=x2, #1
asr x2, x2, #1

# qhasm:     free m1

# qhasm:     free ff

# qhasm: mem64[input_x0 + BYTE_IDX_DELTA] = DELTA
# asm 1: str <DELTA=int64#6, [<input_x0=int64#1, #BYTE_IDX_DELTA]
# asm 2: str <DELTA=x5, [<input_x0=x0, #BYTE_IDX_DELTA]
str x5, [x0, #BYTE_IDX_DELTA]

# qhasm: CONST_2P20A2P41 = mem64[input_x0 + BYTE_IDX_CONST_2P20A2P41]
# asm 1: ldr >CONST_2P20A2P41=int64#4, [<input_x0=int64#1, #BYTE_IDX_CONST_2P20A2P41]
# asm 2: ldr >CONST_2P20A2P41=x3, [<input_x0=x0, #BYTE_IDX_CONST_2P20A2P41]
ldr x3, [x0, #BYTE_IDX_CONST_2P20A2P41]

# qhasm: CONST_2P20 = mem64[input_x0 + BYTE_IDX_CONST_2P20]
# asm 1: ldr >CONST_2P20=int64#5, [<input_x0=int64#1, #BYTE_IDX_CONST_2P20]
# asm 2: ldr >CONST_2P20=x4, [<input_x0=x0, #BYTE_IDX_CONST_2P20]
ldr x4, [x0, #BYTE_IDX_CONST_2P20]

# qhasm: v = FUV + CONST_2P20A2P41
# asm 1: add >v=int64#6,<FUV=int64#2,<CONST_2P20A2P41=int64#4
# asm 2: add >v=x5,<FUV=x1,<CONST_2P20A2P41=x3
add x5,x1,x3

# qhasm: v = v signed>> 42
# asm 1: asr >v=int64#6, <v=int64#6, #42
# asm 2: asr >v=x5, <v=x5, #42
asr x5, x5, #42

# qhasm: u = FUV + CONST_2P20
# asm 1: add >u=int64#2,<FUV=int64#2,<CONST_2P20=int64#5
# asm 2: add >u=x1,<FUV=x1,<CONST_2P20=x4
add x1,x1,x4

# qhasm: u = u << 22
# asm 1: lsl >u=int64#2, <u=int64#2, #22
# asm 2: lsl >u=x1, <u=x1, #22
lsl x1, x1, #22

# qhasm: u = u signed>> 43
# asm 1: asr >u=int64#2, <u=int64#2, #43
# asm 2: asr >u=x1, <u=x1, #43
asr x1, x1, #43

# qhasm: s = GRS + CONST_2P20A2P41
# asm 1: add >s=int64#4,<GRS=int64#3,<CONST_2P20A2P41=int64#4
# asm 2: add >s=x3,<GRS=x2,<CONST_2P20A2P41=x3
add x3,x2,x3

# qhasm: s = s signed>> 42
# asm 1: asr >s=int64#4, <s=int64#4, #42
# asm 2: asr >s=x3, <s=x3, #42
asr x3, x3, #42

# qhasm: r = GRS + CONST_2P20
# asm 1: add >r=int64#3,<GRS=int64#3,<CONST_2P20=int64#5
# asm 2: add >r=x2,<GRS=x2,<CONST_2P20=x4
add x2,x2,x4

# qhasm: r = r << 22
# asm 1: lsl >r=int64#3, <r=int64#3, #22
# asm 2: lsl >r=x2, <r=x2, #22
lsl x2, x2, #22

# qhasm: r = r signed>> 43
# asm 1: asr >r=int64#3, <r=int64#3, #43
# asm 2: asr >r=x2, <r=x2, #43
asr x2, x2, #43

# qhasm: mem64[input_x0 + BYTE_IDX_u] = u
# asm 1: str <u=int64#2, [<input_x0=int64#1, #BYTE_IDX_u]
# asm 2: str <u=x1, [<input_x0=x0, #BYTE_IDX_u]
str x1, [x0, #BYTE_IDX_u]

# qhasm: mem64[input_x0 + BYTE_IDX_v] = v
# asm 1: str <v=int64#6, [<input_x0=int64#1, #BYTE_IDX_v]
# asm 2: str <v=x5, [<input_x0=x0, #BYTE_IDX_v]
str x5, [x0, #BYTE_IDX_v]

# qhasm: mem64[input_x0 + BYTE_IDX_r] = r
# asm 1: str <r=int64#3, [<input_x0=int64#1, #BYTE_IDX_r]
# asm 2: str <r=x2, [<input_x0=x0, #BYTE_IDX_r]
str x2, [x0, #BYTE_IDX_r]

# qhasm: mem64[input_x0 + BYTE_IDX_s] = s
# asm 1: str <s=int64#4, [<input_x0=int64#1, #BYTE_IDX_s]
# asm 2: str <s=x3, [<input_x0=x0, #BYTE_IDX_s]
str x3, [x0, #BYTE_IDX_s]

# qhasm: return
ret
