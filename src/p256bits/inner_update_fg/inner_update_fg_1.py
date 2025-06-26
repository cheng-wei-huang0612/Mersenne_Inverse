import os

script_name = os.path.basename(__file__)
base_name, _ = os.path.splitext(script_name)

code = f"""
# Generate by {script_name}
"""

# Your code begins from here

code += """
include "slot.h"
enter inner_update_fg

int64 pointer_tmp
input pointer_tmp
"""

code += """

int64 u
int64 v
int64 r
int64 s

int64 f
int64 f_lo
int64 f_hi
int64 f_new
int64 g
int64 g_lo
int64 g_hi
int64 g_new

f = mem64[pointer_tmp + BYTE_IDX_f]
g = mem64[pointer_tmp + BYTE_IDX_g]
u = mem64[pointer_tmp + BYTE_IDX_u]
v = mem64[pointer_tmp + BYTE_IDX_v]
r = mem64[pointer_tmp + BYTE_IDX_r]
s = mem64[pointer_tmp + BYTE_IDX_s]

f_lo = f & 0xFFFFFFFF
f_hi = f signed>> 32

g_lo = g & 0xFFFFFFFF
g_hi = g signed>> 32

int64 buf0
int64 buf1
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
"""

code += """
mem64[pointer_tmp + BYTE_IDX_f] = f_new
mem64[pointer_tmp + BYTE_IDX_g] = g_new
"""


code += """
return
"""

# Your code ends here

output_q_file = f"{base_name}.q"
output_s_file = f"{base_name}.S"

with open(output_q_file, "w") as f:
    f.write(code)


os.system(f"qhasm-aarch64-align < {output_q_file} > {output_s_file}")   
print(f"Generated {output_q_file}")
print(f"Generated {output_s_file}")