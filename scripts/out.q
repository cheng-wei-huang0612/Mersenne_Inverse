int64 CONST_2P20
int64 CONST_2P20A2P41
int64 CONST_2P41
int64 CONST_2P62
int64 DELTA
int64 FUV
int64 GRS
int64 buf0
int64 buf1
int64 f
int64 f_hi
int64 f_lo
int64 f_new
int64 ff
int64 g
int64 g_hi
int64 g_lo
int64 g_new
int64 m1
int64 r
int64 s
int64 u
int64 v

enter result

# Generate by divstepx20_1.py







CONST_2P41 = mem64[input_x0 + BYTE_IDX_CONST_2P41]
CONST_2P62 = mem64[input_x0 + BYTE_IDX_CONST_2P62]

f = mem64[input_x0 + BYTE_IDX_f]
g = mem64[input_x0 + BYTE_IDX_g]
DELTA = mem64[input_x0 + BYTE_IDX_DELTA]

f = f & 0xFFFFF
g = g & 0xFFFFF
FUV = f - CONST_2P41
GRS = g - CONST_2P62



    # loop 1

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 2

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 3

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 4

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 5

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 6

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 7

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 8

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 9

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 10

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 11

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 12

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 13

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 14

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 15

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 16

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 17

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 18

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 19

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


    # loop 20

    m1 = DELTA - 1 
    GRS & 1
    ff = FUV if Z=0 else 0
    m1 & (GRS >>> 1)
    DELTA = m1 if N=0 else -DELTA
    FUV = GRS if N=1 else FUV
    ff = ff if N=0 else -ff
    GRS = GRS + ff
    GRS = GRS signed>> 1

    free m1
    free ff


mem64[input_x0 + BYTE_IDX_DELTA] = DELTA



# Generate by extraction_to_uvrs_1.py








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





# Generate by inner_update_fg_1.py






f = mem64[input_x0 + BYTE_IDX_f]
g = mem64[input_x0 + BYTE_IDX_g]

f_lo = f & 0xFFFFFFFF
f_hi = f signed>> 32

g_lo = g & 0xFFFFFFFF
g_hi = g signed>> 32

buf0 = u * f_lo
buf0 = v * g_lo + buf0
buf0 = buf0 signed>> 20

buf1 = u * f_hi
buf1 = v * g_hi + buf1
buf1 = buf1 << 12
f_new = buf0 + buf1


buf0 = r * f_lo
buf0 = s * g_lo + buf0
buf0 = buf0 signed>> 20

buf1 = r * f_hi
buf1 = s * g_hi + buf1
buf1 = buf1 << 12
g_new = buf0 + buf1

mem64[input_x0 + BYTE_IDX_f] = f_new
mem64[input_x0 + BYTE_IDX_g] = g_new


return
