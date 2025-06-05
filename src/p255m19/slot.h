
/* ⚠️ 自動產生，請勿手改
 * 來源：slots.list
 */
#ifndef SLOTS_H
#define SLOTS_H

#define IDX_CONST_2P32M1         0
#define IDX_CONST_2P30M1         1
#define IDX_CONST_2P30M19        2
#define IDX_CONST_2P15M1         3
#define IDX_X0                   4
#define IDX_X1                   5
#define IDX_X2                   6
#define IDX_X3                   7
#define IDX_INV0                 8
#define IDX_INV1                 9
#define IDX_INV2                 10
#define IDX_INV3                 11
#define IDX_F0                   12
#define IDX_F1                   13
#define IDX_F2                   14
#define IDX_F3                   15
#define IDX_F4                   16
#define IDX_F5                   17
#define IDX_F6                   18
#define IDX_F7                   19
#define IDX_F8                   20
#define IDX_G0                   21
#define IDX_G1                   22
#define IDX_G2                   23
#define IDX_G3                   24
#define IDX_G4                   25
#define IDX_G5                   26
#define IDX_G6                   27
#define IDX_G7                   28
#define IDX_G8                   29
#define IDX_V0                   30
#define IDX_V1                   31
#define IDX_V2                   32
#define IDX_V3                   33
#define IDX_V4                   34
#define IDX_V5                   35
#define IDX_V6                   36
#define IDX_V7                   37
#define IDX_V8                   38
#define IDX_S0                   39
#define IDX_S1                   40
#define IDX_S2                   41
#define IDX_S3                   42
#define IDX_S4                   43
#define IDX_S5                   44
#define IDX_S6                   45
#define IDX_S7                   46
#define IDX_S8                   47
#define IDX_DELTA                48
#define IDX_UU                   49
#define IDX_VV                   50
#define IDX_RR                   51
#define IDX_SS                   52
#define IDX_f                    53
#define IDX_g                    54
#define IDX_FUV                  55
#define IDX_GRS                  56
#define IDX_SCR0                 57
#define IDX_SCR1                 58
#define IDX_SCR2                 59
#define IDX_SCR3                 60
#define IDX_G0AND1               61
#define IDX_COND                 62
#define IDX_CMASK                63
#define IDX_NMASK                64
#define IDX_NEG_FUV              65
#define IDX_DELTA_SWAP           66
#define IDX_TMP                  67
#define IDX_u                    68
#define IDX_v                    69
#define IDX_r                    70
#define IDX_s                    71

#define SLOTS_TOTAL               72
#define SLOT_OFFSET(idx)        ((idx) << 3)  /* 64-bit array -> bytes */

#endif /* SLOTS_H */
