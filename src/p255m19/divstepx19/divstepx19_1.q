
# Generate by divstepx19_1.py

enter divstepx19

include "slot.h"
input input_x0


int64 FUV
int64 GRS
int64 DELTA
int64 f
int64 g
int64 CONST_2P41
int64 CONST_2P62

int64 m1
int64 ff


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


mem64[input_x0 + BYTE_IDX_FUV] = FUV
mem64[input_x0 + BYTE_IDX_GRS] = GRS
mem64[input_x0 + BYTE_IDX_DELTA] = DELTA


return
