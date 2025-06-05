/* slots.h ── 64-bit 連續暫存索引表（9-limb × 30-bit 版本） */
enum {
    IDX_CONST_2P32M1,   /* 2^32 − 1 */
    IDX_CONST_2P30M1,   /* 2^30 − 1 */
    IDX_CONST_2P30M19,  /* 2^30 − 19 */
    IDX_CONST_2P15M1,   /* 2^15 − 1 */
    
    IDX_X0, IDX_X1, IDX_X2, IDX_X3, 
    IDX_INV0, IDX_INV1, IDX_INV2, IDX_INV3, /* 0 – 7 */
    /*─── F.limb[0..8] ───*/                    /*  0 –  8 */
    IDX_F0, IDX_F1, IDX_F2, IDX_F3, IDX_F4,
    IDX_F5, IDX_F6, IDX_F7, IDX_F8,

    /*─── G.limb[0..8] ───*/                    /*  9 – 17 */
    IDX_G0, IDX_G1, IDX_G2, IDX_G3, IDX_G4,
    IDX_G5, IDX_G6, IDX_G7, IDX_G8,

    /*─── V.limb[0..8] ───*/                    /* 18 – 26 */
    IDX_V0, IDX_V1, IDX_V2, IDX_V3, IDX_V4,
    IDX_V5, IDX_V6, IDX_V7, IDX_V8,

    /*─── S.limb[0..8] ───*/                    /* 27 – 35 */
    IDX_S0, IDX_S1, IDX_S2, IDX_S3, IDX_S4,
    IDX_S5, IDX_S6, IDX_S7, IDX_S8,

    /*─── 長壽標量 ───*/                        /* 36 – 40 */
    IDX_DELTA,          /* 36 */
    IDX_UU, IDX_VV, IDX_RR, IDX_SS,             /* 37 – 40 */

    /*─── 常數區 ───*/                          /* 41 – 44 */

    /*─── 中壽標量 ───*/                        /* 45 – 48 */
    IDX_f, IDX_g,                               /* 45 – 46 */
    IDX_FUV, IDX_GRS,                           /* 47 – 48 */

    /*─── Scratch & divstep 內暫存 ───*/        /* 49 – 61 */
    IDX_SCR0, IDX_SCR1, IDX_SCR2, IDX_SCR3,     /* 49 – 52 */
    IDX_G0AND1, IDX_COND, IDX_CMASK, IDX_NMASK, /* 53 – 56 */
    IDX_NEG_FUV, IDX_DELTA_SWAP, IDX_TMP,       /* 57 – 59 */

    /*─── u,v,r,s (抽取後存這裡) ───*/          /* 60 – 63 */
    IDX_u, IDX_v, IDX_r, IDX_s,                 /* 60 – 63 */

    SLOTS_TOTAL                                 /* 64 */
};