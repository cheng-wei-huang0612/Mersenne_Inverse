import os

script_name = os.path.basename(__file__)
base_name, _ = os.path.splitext(script_name)

code = f"""
# Generate by {script_name}
"""

# Your code begins from here

code += """
enter divstepx20
"""

code += """
include "slot.h"
input input_x0
"""


code += """

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


"""


for i in range(20):
    code += f"""
    # loop {i+1}

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

"""

code += """
mem64[input_x0 + BYTE_IDX_FUV] = FUV
mem64[input_x0 + BYTE_IDX_GRS] = GRS
mem64[input_x0 + BYTE_IDX_DELTA] = DELTA

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