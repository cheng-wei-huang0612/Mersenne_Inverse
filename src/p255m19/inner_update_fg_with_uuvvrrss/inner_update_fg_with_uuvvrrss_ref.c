#include <stdint.h>
#include <stdio.h>
#include "slot.h"

#include <gmp.h>
void inner_update_fg_with_uuvvrrss(uint64_t *tmp) {
    int64_t *f = (int64_t *)&tmp[IDX_f];
    int64_t *g = (int64_t *)&tmp[IDX_g];
    uint64_t *uuvvrrss = (uint64_t *)&tmp[IDX_UU];

    int64_t uu = uuvvrrss[0];
    int64_t vv = uuvvrrss[1];
    int64_t rr = uuvvrrss[2];
    int64_t ss = uuvvrrss[3];



    mpz_t mpf, mpf_new, mpg, mpg_new, mpu, mpv, mpr, mps, mpztmp;
    mpz_init(mpf_new);
    mpz_init(mpg_new);
    mpz_init(mpztmp);

    mpz_init_set_si(mpf, *f);
    mpz_init_set_si(mpg, *g);
    mpz_init_set_si(mpu, uu);
    mpz_init_set_si(mpv, vv);
    mpz_init_set_si(mpr, rr);
    mpz_init_set_si(mps, ss);
    
    mpz_mul(mpztmp, mpu, mpf);
    mpz_addmul(mpztmp, mpv, mpg);
    mpz_fdiv_q_2exp(mpf_new, mpztmp, 20);

    mpz_mul(mpztmp, mpr, mpf);
    mpz_addmul(mpztmp, mps, mpg);
    mpz_fdiv_q_2exp(mpg_new, mpztmp, 20);

    *f = mpz_get_si(mpf_new);
    *g = mpz_get_si(mpg_new);

    mpz_clears(mpf, mpg, mpf_new, mpg_new, mpztmp, mpu, mpv ,mpr ,mps, NULL);


}




