# slots.py
from slots_generator import Slot, write_header

# ── Slot 定義：名稱:words:align(optional) ──
SLOTS = [
    # 常數 64-bit
    Slot("CONST_2P32M1", 1),
    Slot("CONST_2P30M1", 1),
    Slot("CONST_2P30M19", 1),
    Slot("CONST_2P15M1", 1),
    Slot("CONST_2P20", 1),
    Slot("CONST_2P41", 1),
    Slot("CONST_2P62", 1),
    Slot("CONST_2P20A2P41", 1),

    Slot("VEC_CONST_2X_2P30M1", 2),

    # 輸入 / 輸出 scalar
    *[Slot(f"X{i}",   1) for i in range(4)],
    *[Slot(f"INV{i}", 1) for i in range(4)],
    *[Slot(f"P{i}",   1) for i in range(4)],

    # 向量資料 128-bit (2 words, 16-byte align)
    *[Slot(f"VEC_F{i}", 1) for i in range(9)],
    *[Slot(f"VEC_G{i}", 1) for i in range(9)],
    *[Slot(f"VEC_V{i}", 1) for i in range(9)],
    *[Slot(f"VEC_S{i}", 1) for i in range(9)],
    *[Slot(f"VEC_P{i}", 1) for i in range(9)],

    Slot("VEC_F0_F1_G0_G1", 2),  
    Slot("VEC_F2_F3_G2_G3", 2),
    Slot("VEC_F4_F5_G4_G5", 2),
    Slot("VEC_F6_F7_G6_G7", 2),
    Slot("VEC_F8_F9_G8_G9", 2),
    Slot("VEC_V0_V1_S0_S1", 2),
    Slot("VEC_V2_V3_S2_S3", 2),
    Slot("VEC_V4_V5_S4_S5", 2),
    Slot("VEC_V6_V7_S6_S7", 2),
    Slot("VEC_V8_V9_S8_S9", 2),

    Slot("VEC_UU0_RR0_VV0_SS0", 2),  # UU, RR, VV, SS
    Slot("VEC_UU1_RR1_VV1_SS1", 2),  # UU, RR, VV, SS

    

    # 內部運算 scalar
    Slot("DELTA", 1), Slot("UU", 1), Slot("VV", 1),
    Slot("RR", 1),    Slot("SS", 1),
    Slot("f", 1),     Slot("g", 1),
    Slot("FUV", 1),   Slot("GRS", 1),

    # scratch / mask
    *[Slot(f"SCR{i}", 1) for i in range(4)],
    Slot("G0AND1", 1), Slot("COND", 1),
    Slot("CMASK", 1),  Slot("NMASK", 1),
    Slot("NEG_FUV", 1), Slot("DELTA_SWAP", 1),

    # 其他
    Slot("TMP", 1),
    Slot("u", 1), Slot("v", 1), Slot("r", 1), Slot("s", 1),
]

# ── 如果直接執行本檔，產生 slot.h ──
if __name__ == "__main__":
    write_header(SLOTS, "slot.h")