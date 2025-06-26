#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "slot.h"
#include <arm_neon.h>
#include <gmp.h>

/*
256bits prime
requiring 256 * 2.304 divsteps = 589.824 < 590
*/

extern void low60bits(uint64_t *tmp);
extern void divstepx19(uint64_t *tmp);
extern void divstepx20(uint64_t *tmp);
extern void extraction_to_uvrs(uint64_t *tmp);
extern void extraction_to_uuvvrrss(uint64_t *tmp);


extern void inner_update_uuvvrrss(uint64_t *tmp);
extern void inner_update_fg(uint64_t *tmp);
extern void inner_update_fg_with_uuvvrrss(uint64_t *tmp);
extern void update_FG(uint64_t *tmp);
extern void update_VS(uint64_t *tmp);
extern void final_adjustment(uint64_t *tmp);
extern void initialization(uint64_t *tmp);




void cpt_inv(uint64_t *x, uint64_t *inv, uint64_t *P){

    //printf("NEW RUN\n\n");

    // Allocate a big memory buffer as tmp (e.g., 4096 bytes)
    uint64_t *tmp = aligned_alloc(8, SLOTS_TOTAL * sizeof(uint64_t));
    if (!tmp) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    memcpy(&tmp[IDX_X0], x, 4 * sizeof(uint64_t));
    memcpy(&tmp[IDX_P0], P, 4 * sizeof(uint64_t));
    
    // Constant Initialization
    tmp[IDX_CONST_2P32M1] = 0x00000000FFFFFFFFULL; /* 2^32 − 1 */
    tmp[IDX_CONST_2P30M1] = 0x000000003FFFFFFFULL; /* 2^30 − 1 */
    tmp[IDX_CONST_2P30M19]= 0x000000003FFFFFEDULL; /* 2^30 − 19 */
    tmp[IDX_CONST_2P15M1] = 0x0000000000007FFFULL; /* 2^15 − 1 */
    tmp[IDX_CONST_2P20] = 0x0000000000100000ULL; /* 2^20 */
    tmp[IDX_CONST_2P41] = 0x0000020000000000ULL; /* 2^41 */
    tmp[IDX_CONST_2P62] = 0x4000000000000000ULL; /* 2^62 */
    tmp[IDX_CONST_2P20A2P41] = 0x0000020000100000ULL; /* 2^20 + 2^41 */

    tmp[IDX_VEC_CONST_2X_2P30M1] = 0x000000003FFFFFFFULL;
    tmp[IDX_VEC_CONST_2X_2P30M1_HI1] = 0x000000003FFFFFFFULL;

    
    // initlaization
    /* --- initialization: radix-2⁶⁴ → radix-2³⁰ (9 limbs) ------------------ */
{
    const uint64_t MASK30 = 0x3FFFFFFFULL;          /* 2³⁰ − 1 */

    uint64_t x0 = tmp[IDX_X0];   /* least-significant  64 bits */
    uint64_t x1 = tmp[IDX_X1];
    uint64_t x2 = tmp[IDX_X2];
    uint64_t x3 = tmp[IDX_X3];   /* most-significant   64 bits */

    /* g[i] = Σ g[i]·2^(30·i)  ≡  x0 + x1·2⁶⁴ + x2·2¹²⁸ + x3·2¹⁹² */
    tmp[IDX_VEC_G0] =  x0 & MASK30;                               /* bits 0  – 29  */
    tmp[IDX_VEC_G1] = (x0 >> 30)               & MASK30;          /* bits 30 – 59  */
    tmp[IDX_VEC_G2] = ((x0 >> 60) | (x1 << 4)) & MASK30;          /* bits 60 – 89  */
    tmp[IDX_VEC_G3] = (x1 >> 26)               & MASK30;          /* bits 90 – 119 */
    tmp[IDX_VEC_G4] = ((x1 >> 56) | (x2 << 8)) & MASK30;          /* bits 120–149 */
    tmp[IDX_VEC_G5] = (x2 >> 22)               & MASK30;          /* bits 150–179 */
    tmp[IDX_VEC_G6] = ((x2 >> 52) | (x3 << 12))& MASK30;          /* bits 180–209 */
    tmp[IDX_VEC_G7] = (x3 >> 18)               & MASK30;          /* bits 210–239 */
    tmp[IDX_VEC_G8] =  x3 >> 48;                                   /* bits 240–255 (≤ 16 bits) */
}
/* --- initialization: P (radix-2⁶⁴) → F[0..8] (radix-2³⁰) -------------- */
{
    const uint64_t MASK30 = 0x3FFFFFFFULL;          /* 2³⁰ − 1 */

    uint64_t p0 = tmp[IDX_P0];  /* least-significant 64 bits */
    uint64_t p1 = tmp[IDX_P1];
    uint64_t p2 = tmp[IDX_P2];
    uint64_t p3 = tmp[IDX_P3];  /* most-significant  64 bits */

    tmp[IDX_VEC_F0] =  p0 & MASK30;
    tmp[IDX_VEC_F1] = (p0 >> 30)               & MASK30;
    tmp[IDX_VEC_F2] = ((p0 >> 60) | (p1 << 4)) & MASK30;
    tmp[IDX_VEC_F3] = (p1 >> 26)               & MASK30;
    tmp[IDX_VEC_F4] = ((p1 >> 56) | (p2 << 8)) & MASK30;
    tmp[IDX_VEC_F5] = (p2 >> 22)               & MASK30;
    tmp[IDX_VEC_F6] = ((p2 >> 52) | (p3 << 12))& MASK30;
    tmp[IDX_VEC_F7] = (p3 >> 18)               & MASK30;
    tmp[IDX_VEC_F8] =  p3 >> 48;   /* 高 16-bit，足以容納最後 limb */


    memcpy(&tmp[IDX_VEC_P0], &tmp[IDX_VEC_F0], 9 * sizeof(uint64_t));
}
{

    // initialize V <- 0 and S <- 1
    tmp[IDX_VEC_V0_V1_S0_S1] = 0;
    tmp[IDX_VEC_V0_V1_S0_S1_HI1] = 0;
    tmp[IDX_VEC_V2_V3_S2_S3] = 0;
    tmp[IDX_VEC_V2_V3_S2_S3_HI1] = 0;
    tmp[IDX_VEC_V4_V5_S4_S5] = 0;
    tmp[IDX_VEC_V4_V5_S4_S5_HI1] = 0;
    tmp[IDX_VEC_V6_V7_S6_S7] = 0;
    tmp[IDX_VEC_V6_V7_S6_S7_HI1] = 0;
    tmp[IDX_VEC_V8_V9_S8_S9] = 0;
    tmp[IDX_VEC_V8_V9_S8_S9_HI1] = 0;


    tmp[IDX_VEC_V0_V1_S0_S1_HI1] = 1;
}

// data shaping:
    tmp[IDX_VEC_F0_F1_G0_G1] = (tmp[IDX_VEC_F0] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_F1] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F0_F1_G0_G1_HI1] = (tmp[IDX_VEC_G0] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_G1] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F2_F3_G2_G3] = (tmp[IDX_VEC_F2] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_F3] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F2_F3_G2_G3_HI1] = (tmp[IDX_VEC_G2] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_G3] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F4_F5_G4_G5] = (tmp[IDX_VEC_F4] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_F5] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F4_F5_G4_G5_HI1] = (tmp[IDX_VEC_G4] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_G5] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F6_F7_G6_G7] = (tmp[IDX_VEC_F6] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_F7] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F6_F7_G6_G7_HI1] = (tmp[IDX_VEC_G6] & 0xFFFFFFFF) | ((uint64_t)(tmp[IDX_VEC_G7] & 0xFFFFFFFF) << 32);
    tmp[IDX_VEC_F8_F9_G8_G9] = (tmp[IDX_VEC_F8] & 0xFFFFFFFF) ;
    tmp[IDX_VEC_F8_F9_G8_G9_HI1] = (tmp[IDX_VEC_G8] & 0xFFFFFFFF) ;






for (size_t i = 0; i < 10; i++) {
    
    // begin{divstepx59}
    low60bits(tmp);
    
    divstepx20(tmp);
    extraction_to_uvrs(tmp);
    inner_update_fg(tmp);
    
    tmp[IDX_UU] = tmp[IDX_u];
    tmp[IDX_VV] = tmp[IDX_v];
    tmp[IDX_RR] = tmp[IDX_r];
    tmp[IDX_SS] = tmp[IDX_s];
    
    divstepx20(tmp);
    extraction_to_uvrs(tmp);
    inner_update_uuvvrrss(tmp);
    inner_update_fg(tmp);
    
    divstepx19(tmp);
    extraction_to_uvrs(tmp);
    inner_update_uuvvrrss(tmp);
    // end{divstepx59}
    
    update_FG(tmp);
    update_VS(tmp);
    
}

    final_adjustment(tmp);


    
    memcpy(inv, &tmp[IDX_INV0], 4 * sizeof(uint64_t));
    

    // Remember to free the memory when done
    free(tmp);
}


