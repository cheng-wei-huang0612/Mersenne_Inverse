import os

script_name = os.path.basename(__file__)
base_name, _ = os.path.splitext(script_name)

code = f"""
# Generate by {script_name}
"""

# Your code begins from here

code += """
include "slot.h"
enter inner_update_uuvvrrss

int64 pointer_tmp
input pointer_tmp
"""

code += """
int64 u
int64 v
int64 r
int64 s
int64 UU
int64 VV
int64 RR
int64 SS

u = mem64[pointer_tmp + BYTE_IDX_u]
v = mem64[pointer_tmp + BYTE_IDX_v]
r = mem64[pointer_tmp + BYTE_IDX_r]
s = mem64[pointer_tmp + BYTE_IDX_s]
UU = mem64[pointer_tmp + BYTE_IDX_UU]
VV = mem64[pointer_tmp + BYTE_IDX_VV]
RR = mem64[pointer_tmp + BYTE_IDX_RR]
SS = mem64[pointer_tmp + BYTE_IDX_SS]

int64 buf

buf = u * UU
buf = buf + v * RR
mem64[pointer_tmp + BYTE_IDX_UU] = buf

buf = u * VV
buf = buf + v * SS
mem64[pointer_tmp + BYTE_IDX_VV] = buf

buf = r * UU
buf = buf + s * RR
mem64[pointer_tmp + BYTE_IDX_RR] = buf

buf = r * VV
buf = buf + s * SS
mem64[pointer_tmp + BYTE_IDX_SS] = buf


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