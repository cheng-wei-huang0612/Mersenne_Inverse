#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "slot.h"
#include <arm_neon.h>

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



void cpt_inv(uint64_t *x, uint64_t *inv){

    // Allocate a big memory buffer as tmp (e.g., 4096 bytes)
    uint64_t *tmp = aligned_alloc(16, SLOTS_TOTAL * sizeof(uint64_t));
    if (!tmp) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    memcpy(&tmp[IDX_X0], x, 4 * sizeof(uint64_t));
    
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

    
    initialization(tmp);
    
    
    
    for (size_t i = 0; i < 10; i++) {
        
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
        
        
        update_FG(tmp);
        update_VS(tmp);
        
    }
    
    

    final_adjustment(tmp);


    
    memcpy(inv, &tmp[IDX_INV0], 4 * sizeof(uint64_t));
    

    // Remember to free the memory when done
    free(tmp);
}


