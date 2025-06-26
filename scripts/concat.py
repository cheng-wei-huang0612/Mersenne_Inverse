#!/usr/bin/env python3
"""
concat.py — 合併檔案並重組輸出
================================

更新內容（v3）
--------------
* **泛化 store/load 抵銷**：
  - 不再寫死 `input_x0`，改為偵測 *任意* 指標變數 `PTR`：
    * **store** : `mem64[ PTR + BYTE_IDX_VAR ] = VAR`
    * **load**  : `VAR = mem64[ PTR + BYTE_IDX_VAR ]`
  - 只有當 `PTR` 與 `VAR` 皆一致時才視為冗餘搬移
  - 允許中間跨行（仍假設期間沒有對該 (PTR, VAR) 進一步寫入）

輸出骨架維持：
```
int64 ...
reg128 ...

enter result
<內容 (已過濾+抵銷)>

return
```
"""

from __future__ import annotations

import re
import sys
from pathlib import Path
from typing import Iterable, List, Set, Tuple, Dict

# ───────────── Regex 規則 ─────────────
INT64_RE   = re.compile(r"^\s*int64\s+(\w+)")
REG128_RE  = re.compile(r"^\s*reg128\s+(\w+)")
INCLUDE_RE = re.compile(r"^\s*include\s+(\S+)")
INPUT_RE   = re.compile(r"^\s*input\s+(\S+)")
ENTER_RE   = re.compile(r"^\s*enter\b")
RETURN_RE  = re.compile(r"^\s*return\b")

# store / load patterns — 任意 PTR 皆可
STORE_RE = re.compile(r"^\s*mem64\[\s*(\w+)\s*\+\s*BYTE_IDX_(\w+)\s*\]\s*=\s*(\w+)\s*$")
LOAD_RE  = re.compile(r"^\s*(\w+)\s*=\s*mem64\[\s*(\w+)\s*\+\s*BYTE_IDX_(\w+)\s*\]\s*$")

# ───────────── Stage 1：read ─────────────

def read_lines(src_paths: Iterable[Path]) -> List[List[str]]:
    return [Path(p).read_text(encoding="utf-8").splitlines(keepends=True) for p in src_paths]

# ───────────── Stage 2：concat ─────────────

def concat(buffers: List[List[str]]) -> List[str]:
    lines: List[str] = []
    for buf in buffers:
        lines.extend(buf)
    return lines

# ───────────── Stage 2.5：filter & collect ─────────────

def filter_and_collect(lines: Iterable[str]) -> Tuple[List[str], Set[str], Set[str], Set[str], Set[str]]:
    int64_vars, reg128_vars, include_set, input_set = set(), set(), set(), set()
    kept: List[str] = []
    for line in lines:
        if (m := INT64_RE.match(line)):
            int64_vars.add(m.group(1)); continue
        if (m := REG128_RE.match(line)):
            reg128_vars.add(m.group(1)); continue
        if (m := INCLUDE_RE.match(line)):
            include_set.add(m.group(1)); continue
        if (m := INPUT_RE.match(line)):
            input_set.add(m.group(1)); continue
        if ENTER_RE.match(line) or RETURN_RE.match(line):
            continue
        kept.append(line)
    return kept, int64_vars, reg128_vars, include_set, input_set

# ───────────── Stage 3a：跨行抵銷 store-load ─────────────

def cancel_mem_pairs(lines: List[str]) -> List[str]:
    """移除冗餘 mem64 store→load。 ptr 與 var 必須全相符。"""
    pending_store: Dict[Tuple[str, str], int] = {}  # (ptr,var) -> line idx
    drop_idx: Set[int] = set()

    for idx, line in enumerate(lines):
        if (sm := STORE_RE.match(line)):
            ptr, idx_var, rhs_var = sm.groups()
            if idx_var == rhs_var:
                pending_store.setdefault((ptr, rhs_var), idx)
            continue
        if (lm := LOAD_RE.match(line)):
            lhs_var, ptr, idx_var = lm.groups()
            if lhs_var == idx_var:
                key = (ptr, lhs_var)
                if key in pending_store:
                    # 刪除 store 與 load
                    drop_idx.update({pending_store.pop(key), idx})
    # 重建
    return [ln for i, ln in enumerate(lines) if i not in drop_idx]

# ───────────── Stage 3b：build header & footer ─────────────

def build_header(int64_vars: Set[str], reg128_vars: Set[str]) -> List[str]:
    header = [f"int64 {v}\n" for v in sorted(int64_vars)]
    header += [f"reg128 {v}\n" for v in sorted(reg128_vars)]
    header += ["\n", "enter result\n"]
    return header

FOOTER = ["\n", "return\n"]

# ───────────── Stage 4：write ─────────────

def write(dest: Path, lines: List[str]) -> None:
    dest.write_text("".join(lines), encoding="utf-8")

# ───────────── CLI 入口 ─────────────

def main(argv: List[str] | None = None) -> None:
    argv = argv or sys.argv[1:]
    if len(argv) < 3:
        print("Usage: python3 concat.py DEST SRC1 SRC2 [SRC3 ...]", file=sys.stderr)
        sys.exit(1)

    dest = Path(argv[0])
    srcs = [Path(p) for p in argv[1:]]

    if dest in srcs:
        print("Error: DEST 不可同時作為來源檔案", file=sys.stderr)
        sys.exit(1)

    try:
        buffers = read_lines(srcs)
        lines   = concat(buffers)
        filtered, int64_vars, reg128_vars, include_set, input_set = filter_and_collect(lines)
        filtered = cancel_mem_pairs(filtered)
        final_output = build_header(int64_vars, reg128_vars) + filtered + FOOTER
        write(dest, final_output)

        # debug prints
        print("int64_vars :", sorted(int64_vars))
        print("reg128_vars:", sorted(reg128_vars))
        print("include_set:", sorted(include_set))
        print("input_set  :", sorted(input_set))
    except Exception as exc:
        print(f"Error: {exc}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
