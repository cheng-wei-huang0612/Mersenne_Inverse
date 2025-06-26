#!/usr/bin/env python3
from __future__ import annotations
import argparse, subprocess, sys, time, itertools
from pathlib import Path
from rich.console import Console
console = Console()

# ─── 路徑 ───────────────────────────────────────────
ROOT     = Path(__file__).resolve().parent.parent
SRCROOT  = ROOT / "src" / "p255m19"
BUILDDIR = ROOT / "build"
BUILDDIR.mkdir(exist_ok=True)

# ─── 編譯旗標 ───────────────────────────────────────
CC, CFLAGS, LDFLAGS = "gcc", ["-O3", f"-I{SRCROOT}"], ["-lgmp"]

# ─── «唯一的真·表格» ───────────────────────────────
MODULES = {
    # key            dir_name                     versions
    "div19":          ("divstepx19",              ["ref", "1"]),
    "div20":          ("divstepx20",              ["ref", "1"]),
    "ifg":            ("inner_update_fg",         ["ref", "1"]),
    "uvrs":           ("extraction_to_uvrs",      ["ref", "1"]),
    "uuvvrrss":       ("inner_update_uuvvrrss",   ["ref", "1"]),
    "low60":          ("low60bits",               ["ref", "1"]),
    "initialization": ("initialization",          ["ref",]),   # 已預留「1」
    "final_adjustment":("final_adjustment",       ["ref",]),
    "update_FG":      ("update_FG",               ["ref",]),
    "update_VS":      ("update_VS",               ["ref",]),
}

COMMON_SRC = [
    SRCROOT / "p255m19_inv.c",
    ROOT / "tests" / "helper.c",
    ROOT / "tests" / "random_test.c",
]

# ─── 基本工具 ───────────────────────────────────────
def sh(cmd: list[str]): subprocess.run(cmd, check=True)
def obj(p: Path) -> Path: return BUILDDIR / f"{p.stem}.o"
def compile_all():
    sources = list(SRCROOT.rglob("*.c")) + list(SRCROOT.rglob("*.S")) + COMMON_SRC
    t0 = time.perf_counter()
    for s in sources: sh([str(CC), *CFLAGS, "-c", str(s), "-o", str(obj(s))])
    console.log(f"[green]✓ compiled {len(sources)} files in {time.perf_counter()-t0:.2f}s")

def pick(key: str, ver: str) -> Path:
    dir_name, _ = MODULES[key]
    opath = BUILDDIR / f"{dir_name}_{ver}.o"
    if not opath.exists():
        console.print(f"[red]{opath} not found, run 'compile' first"); sys.exit(1)
    return opath

def link_run(tag: str, objects: list[Path]):
    exe = ROOT / f"rand_{tag}"
    sh([CC, *(str(o) for o in objects), *LDFLAGS, "-o", str(exe)])
    console.log(f"[cyan]▶ {exe.name}")
    sh([str(exe)])

# ─── CLI ───────────────────────────────────────────
def parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser("p255m19 build")
    sub = p.add_subparsers(dest="cmd", required=True)
    sub.add_parser("compile"); sub.add_parser("clean")

    prun = sub.add_parser("run")
    for k, (_, vs) in MODULES.items():
        prun.add_argument(f"--{k}", choices=vs, default=vs[0])
    sub.add_parser("run_all")
    return p

# ─── 主流程 ─────────────────────────────────────────
def main():
    args = parser().parse_args()
    if args.cmd == "compile": compile_all(); return
    if args.cmd == "clean":
        for f in BUILDDIR.glob("*.o"): f.unlink()
        for f in ROOT.glob("rand_*"):  f.unlink()
        console.print("[yellow]clean done"); return

    compile_all()
    if args.cmd == "run":
        combo = {k: getattr(args, k) for k in MODULES}
        run_combo(combo)
    else:               # run_all
        keys, variants = zip(*[(k, v[1]) for k,v in MODULES.items()])
        for vs in itertools.product(*variants):
            run_combo(dict(zip(keys, vs)))

def run_combo(combo: dict[str,str]):
    tag  = "-".join(f"{k}{v}" for k,v in combo.items())
    var_objs = [pick(k, combo[k]) for k in combo]
    common   = [obj(s) for s in COMMON_SRC]
    link_run(tag, var_objs + common)

if __name__ == "__main__":
    try: main()
    except subprocess.CalledProcessError as e:
        console.print(f"[red]✗ build failed ({e.returncode})"); sys.exit(e.returncode)