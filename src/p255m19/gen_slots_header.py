#!/usr/bin/env python3
"""
把 slots.list 轉成 slots.h
Usage: python gen_slots_header.py include/slots.list include/slots.h
"""
import sys, re, pathlib, textwrap

src  = pathlib.Path(sys.argv[1]).read_text()
dest = pathlib.Path(sys.argv[2])

names = re.split(r"\s+", src.strip())
with dest.open("w") as f:
    guard = "SLOTS_H"
    f.write(textwrap.dedent(f"""
        /* ⚠️ 自動產生，請勿手改
         * 來源：{sys.argv[1]}
         */
        #ifndef {guard}
        #define {guard}

        """))

    for i, n in enumerate(names):
        f.write(f"#define {n:<24} {i}\n")

    f.write("\n/* Byte offsets (64-bit array → bytes) */\n")
    for i, n in enumerate(names):
        f.write(f"#define BYTE_{n:<19} {i << 3}\n")

    f.write(f"\n#define SLOTS_TOTAL               {len(names)}\n")
    f.write("#define SLOT_OFFSET(idx)        ((idx) << 3)  /* 64-bit array -> bytes */\n")
    f.write(f"\n#endif /* {guard} */\n")

print(f"[gen] {dest}  ⟵  {len(names)} symbols")