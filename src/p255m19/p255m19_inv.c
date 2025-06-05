#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include "slot.h"

extern void divstep(int64_t *delta, int64_t *fuv, int64_t *grs);
extern void extraction(int64_t *uvrs, int64_t *fuv, int64_t *grs);
extern void inner_update(int64_t *f, int64_t *g, int64_t *uuvvrrss, int64_t *uvrs );
/* 以 30-bit limb 讀 tmp → 填 mpz */
static void mpz_from_tmp_big30(mpz_t z, const uint64_t *T, int base_idx)
{
    mpz_set_ui(z, 0);
    mpz_set_si(z, (int64_t)(int32_t)(T[base_idx + 8] & 0xFFFFFFFF)); /* limb[8] 低 32 bit */
    for (int i = 8-1; i >= 0; i--) {
        mpz_mul_2exp(z, z, 30);                 /* << 30  */
        mpz_add_ui(z, z, T[base_idx + i]);
    }
}

/* mpz → 回寫到 tmp[base]…tmp[base+8]（低 limb 在前） */
static void tmp_big30_from_mpz(uint64_t *T, int base_idx, const mpz_t z)
{
    mpz_t tmp;  mpz_init_set(tmp, z);
    for (int i = 0; i < 8; i++) {
        T[base_idx + i] = mpz_get_si(tmp) & 0x3FFFFFFF;
        mpz_fdiv_q_2exp(tmp, tmp, 30);          /* >> 30  */
    }
    T[base_idx + 8] = mpz_get_si(tmp) & 0xFFFFFFFF;
    mpz_clear(tmp);
}


void cpt_inv(uint64_t *x, uint64_t *inv){

    // Allocate a big memory buffer as tmp (e.g., 4096 bytes)
    uint64_t *tmp = aligned_alloc(64, SLOTS_TOTAL * sizeof(uint64_t));
    if (!tmp) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Constant Initialization
    tmp[IDX_CONST_2P32M1] = 0x00000000FFFFFFFFULL; /* 2^32 − 1 */
    tmp[IDX_CONST_2P30M1] = 0x000000003FFFFFFFULL; /* 2^30 − 1 */
    tmp[IDX_CONST_2P30M19]= 0x000000003FFFFFEDULL; /* 2^30 − 19 */
    tmp[IDX_CONST_2P15M1] = 0x0000000000007FFFULL; /* 2^15 − 1 */
    
    // convert x into 9-limb radix 2^30 representation
    uint64_t x_copy[4];
    for (int i = 0; i < 4; i++) {
        x_copy[i] = x[i];
    }

    for(int i = 0; i < 9; i++) {
        tmp[IDX_G0 + i] = (uint32_t)(x_copy[0] & 0x3FFFFFFF);


        uint64_t t0 = x_copy[0];
        uint64_t t1 = x_copy[1];
        uint64_t t2 = x_copy[2];
        uint64_t t3 = x_copy[3];

        x_copy[0] = (t0 >> 30) | (t1 << 34);
        x_copy[1] = (t1 >> 30) | (t2 << 34);
        x_copy[2] = (t2 >> 30) | (t3 << 34);
        x_copy[3] =  t3 >> 30;
    }

    // initialize F <- P
    tmp[IDX_F0] = (uint32_t)(tmp[IDX_CONST_2P30M19]);
    for (size_t i = 1; i < 8; i++)
    {
        tmp[IDX_F0 + i] = (uint32_t)(tmp[IDX_CONST_2P30M1]);
    }
    tmp[IDX_F8] = (uint32_t)(tmp[IDX_CONST_2P15M1]);

    // initialize V <- 0 and S <- 1
    for (size_t i = 0; i < 9; i++)
    {
        tmp[IDX_V0 + i] = 0;
        tmp[IDX_S0 + i] = 0;
    }
    tmp[IDX_S0] = 1;

    // initialize delta
    tmp[IDX_DELTA]  = 1;

    for (size_t i = 0; i < 10; i++) {
        // low 60
        tmp[IDX_f] = (int64_t)(((tmp[IDX_F1] & 0x3FFFFFFF) << 30) | (tmp[IDX_F0] & 0x3FFFFFFF));
        tmp[IDX_g] = (int64_t)(((tmp[IDX_G1] & 0x3FFFFFFF) << 30) | (tmp[IDX_G0] & 0x3FFFFFFF));


        tmp[IDX_UU] = 1;
        tmp[IDX_VV] = 0;
        tmp[IDX_RR] = 0;
        tmp[IDX_SS] = 1;

        for (size_t i = 0; i < 3; i++) {
            tmp[IDX_FUV] = (tmp[IDX_f] & 0xFFFFF) - ( (int64_t) 1 << 41 );
            tmp[IDX_GRS] = (tmp[IDX_g] & 0xFFFFF) - ( (int64_t) 1 << 62 );

            for (size_t i = 0; i < 20; i++) {
                divstep(
                    (int64_t *) &tmp[IDX_DELTA], 
                    (int64_t *) &tmp[IDX_FUV], 
                    (int64_t *) &tmp[IDX_GRS]
                );
            }


            // extraction 
            extraction(
                (int64_t *) &tmp[IDX_u], 
                (int64_t *) &tmp[IDX_FUV], 
                (int64_t *) &tmp[IDX_GRS]
            );

            // update_fg
            inner_update(
                (int64_t *) &tmp[IDX_f], 
                (int64_t *) &tmp[IDX_g], 
                (int64_t *) &tmp[IDX_UU], 
                (int64_t *) &tmp[IDX_u]
            );
        }

        // update_FG
        {
            /* 1. 轉 F,G → mpz */
            mpz_t mpF, mpG;                      mpz_inits(mpF, mpG, NULL);
            mpz_from_tmp_big30(mpF, tmp, IDX_F0);
            mpz_from_tmp_big30(mpG, tmp, IDX_G0);

            /* 2. 係數 uu vv rr ss */
            mpz_t mpuu, mpvv, mprr, mpss;        mpz_inits(mpuu, mpvv, mprr, mpss, NULL);
            mpz_set_si(mpuu, (int64_t)tmp[IDX_UU]);
            mpz_set_si(mpvv, (int64_t)tmp[IDX_VV]);
            mpz_set_si(mprr, (int64_t)tmp[IDX_RR]);
            mpz_set_si(mpss, (int64_t)tmp[IDX_SS]);

            /* 3. F_new & G_new  (>>60) */
            mpz_t acc, mpF_new, mpG_new;         mpz_inits(acc, mpF_new, mpG_new, NULL);

            mpz_mul(acc, mpuu, mpF);             /* uu*F            */
            mpz_addmul(acc, mpvv, mpG);          /* + vv*G          */
            mpz_fdiv_q_2exp(mpF_new, acc, 60);   /* >> 60           */

            mpz_mul(acc, mprr, mpF);             /* rr*F            */
            mpz_addmul(acc, mpss, mpG);          /* + ss*G          */
            mpz_fdiv_q_2exp(mpG_new, acc, 60);   /* >> 60           */

            /* 4. 回寫到 tmp[] */
            tmp_big30_from_mpz(tmp, IDX_F0, mpF_new);
            tmp_big30_from_mpz(tmp, IDX_G0, mpG_new);

            /* 5. 清理 */
            mpz_clears(mpF, mpG, mpF_new, mpG_new,
                    mpuu, mpvv, mprr, mpss, acc, NULL);
        }


        // update_VS
        {
            /* 讀 uu vv rr ss (64-bit) ----------------------------------- */
            int64_t uu = (int64_t)tmp[IDX_UU];
            int64_t vv = (int64_t)tmp[IDX_VV];
            int64_t rr = (int64_t)tmp[IDX_RR];
            int64_t ss = (int64_t)tmp[IDX_SS];

            /* ----------- mpz 變數宣告 / 轉入 --------------------------- */
            mpz_t mpV, mpS;
            mpz_inits(mpV, mpS, NULL);
            mpz_from_tmp_big30(mpV, tmp, IDX_V0);
            mpz_from_tmp_big30(mpS, tmp, IDX_S0);

            mpz_t mpuu, mpvv, mprr, mpss;
            mpz_inits(mpuu, mpvv, mprr, mpss, NULL);
            mpz_set_si(mpuu, uu);
            mpz_set_si(mpvv, vv);
            mpz_set_si(mprr, rr);
            mpz_set_si(mpss, ss);

            /* ----------- 模數 P (全域 big30_t P → mpz，一次性建立即可) */
            static mpz_t mpP;
            mpz_init(mpP);
            mpz_set_ui(mpP, 1);
            mpz_mul_2exp(mpP, mpP, 255); // P = 2^255
            mpz_sub_ui(mpP, mpP, 19); // P = 2^255 - 19


            /* ----------- (2^60)^-1 (mod P) 只算一次 ------------------- */
            static mpz_t mpInv2p60;
            static int   inv_ready = 0;
            if (!inv_ready) {
                mpz_init_set_ui(mpInv2p60, 1);
                mpz_mul_2exp(mpInv2p60, mpInv2p60, 60);  /* 2^60          */
                mpz_invert(mpInv2p60, mpInv2p60, mpP);   /* inverse mod P */
                inv_ready = 1;
            }

            /* ----------- V_new = (uu·V + vv·S) * inv2p60 mod P -------- */
            mpz_t tmp1;  mpz_init(tmp1);
            mpz_mul(tmp1, mpuu, mpV);
            mpz_addmul(tmp1, mpvv, mpS);
            mpz_mul(tmp1, tmp1, mpInv2p60);
            mpz_mod(tmp1, tmp1, mpP);         /* tmp1 = V_new */

            /* ----------- S_new = (rr·V + ss·S) * inv2p60 mod P -------- */
            mpz_t tmp2;  mpz_init(tmp2);
            mpz_mul(tmp2, mprr, mpV);
            mpz_addmul(tmp2, mpss, mpS);
            mpz_mul(tmp2, tmp2, mpInv2p60);
            mpz_mod(tmp2, tmp2, mpP);         /* tmp2 = S_new */

            /* ----------- 回寫到 tmp[] ---------------------------------- */
            tmp_big30_from_mpz(tmp, IDX_V0, tmp1);
            tmp_big30_from_mpz(tmp, IDX_S0, tmp2);

            /* ----------- 清理 (除 mpP/mpInv2p60) ---------------------- */
            mpz_clears(mpV, mpS, mpuu, mpvv, mprr, mpss, tmp1, tmp2, NULL);
        }

        
    }
    
    // final adjustment
    {
        /* 1. 轉 V → mpz */
        mpz_t mpV;      mpz_init(mpV);
        mpz_from_tmp_big30(mpV, tmp, IDX_V0);

        /* 2. 取 sign(F)  (limb0 最低 30 bit，帶符號延伸到 32 bit) */
        int32_t sign_F = (int32_t)tmp[IDX_F0];   /* 取 32 bit  */
        sign_F <<= 2;   sign_F >>= 2;            /* 保留最低 30 bit 的符號 */

        /* 3. 若 sign_F 為 −1 則取模補正；若 0/1 則照乘 */
        mpz_t mpSign;   mpz_init_set_si(mpSign, sign_F);

        /* 4. 模數 P（靜態一次性建） */
        static mpz_t mpP;
        mpz_init(mpP);
        mpz_set_ui(mpP, 1);
        mpz_mul_2exp(mpP, mpP, 255); // P = 2^255
        mpz_sub_ui(mpP, mpP, 19); // P = 2^255 - 19

        /* 5. V ← V * sign(F)  (mod P) */
        mpz_mul(mpV, mpV, mpSign);
        mpz_mod(mpV, mpV, mpP);

        /* 6. 回寫到 tmp[] */
        tmp_big30_from_mpz(tmp, IDX_V0, mpV);

        for (int i = 8; i >= 0; i--) {
            // 整個 t[3..0] << 30
            // new_t[3] = (old_t[3] << 30) | (old_t[2] >> (64-30))
            // new_t[2] = (old_t[2] << 30) | (old_t[1] >> (64-30))
            // ... 類似 shiftLeft30，這裡直接手動 inline

            uint64_t old0 = inv[0];
            uint64_t old1 = inv[1];
            uint64_t old2 = inv[2];
            uint64_t old3 = inv[3];

            inv[3] = (old3 << 30) | (old2 >> 34);
            inv[2] = (old2 << 30) | (old1 >> 34);
            inv[1] = (old1 << 30) | (old0 >> 34);
            inv[0] = (old0 << 30);

            // 再加上 x->limb[i] (最低 30 bits)，以 64-bit 來裝沒問題
            inv[0] |= (uint64_t) tmp[IDX_V0 + i] & 0x3FFFFFFF; 
        }


        mpz_clears(mpV, mpSign, NULL);
    }




    // Remember to free the memory when done
    free(tmp);

    
}


