# slots_generator.py
from __future__ import annotations
from dataclasses import dataclass
from pathlib import Path
from typing import List

# ── 1. Slot 型別 ─────────────────────────────
@dataclass(frozen=True)
class Slot:
    name:   str          # 巨集名稱（不含 IDX_ / BYTE_IDX_）
    words:  int          # 1 word = 64 bits
    align:  int = 8      # 位元組對齊：8、16、32…
    comment: str = ""    # 可選備註，僅供閱讀

# ── 2. 產生 slots.h ─────────────────────────


def write_header(slots: List[Slot],
                 outfile: str | Path,
                 guard: str = "SLOTS_H") -> None:
    outfile = Path(outfile)

    header = [
        "/* ⚠️ Auto-generated; DO NOT EDIT. */",
        f"#ifndef {guard}",
        f"#define {guard}",
        ""
    ]

    idx_lines:  List[str] = []      # <── 暫存 IDX_ 巨集
    byte_lines: List[str] = []      # <── 暫存 BYTE_IDX_ 巨集
    idx = 0

    for slot in slots:
        blk  = slot.align // 8
        pad  = (blk - (idx % blk)) % blk
        for _ in range(pad):
            idx_lines.append(f"#define IDX_PAD_{idx:<22} {idx}")
            # 不需要 BYTE_ 對應
            idx += 1

        base = idx
        idx_lines.append(f"#define IDX_{slot.name:<22} {base}")
        byte_lines.append(f"#define BYTE_IDX_{slot.name:<16} {base << 3}")

        # 產生高位段 (words > 1)
        for w in range(1, slot.words):
            hi  = f"{slot.name}_HI{w}"
            idx_lines.append(f"#define IDX_{hi:<22} {base + w}")
            byte_lines.append(f"#define BYTE_IDX_{hi:<16} {(base + w) << 3}")

        idx += slot.words

    footer = [
        "",
        f"#define SLOTS_TOTAL               {idx}",
        "#define SLOT_OFFSET(idx)        ((idx) << 3)",
        "",
        f"#endif /* {guard} */",
        ""
    ]

    # 依序寫出：header → IDX_… → 空行 → BYTE_IDX_… → footer
    outfile.write_text(
        "\n".join(header + idx_lines + [""] + byte_lines + footer)
    )
    print(f"[slots] Wrote {outfile}  ({len(slots)} symbols, {idx} total slots)")