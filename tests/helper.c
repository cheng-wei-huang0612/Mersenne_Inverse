
#include <gmp.h>
#include <stdint.h>



void uint256_from_mpz(uint64_t dst[4], const mpz_t src) {
        mpz_t temp;
        mpz_init_set(temp, src);
        for (int i = 0; i < 4; i++) {
            dst[i] = mpz_get_ui(temp);
            mpz_fdiv_q_2exp(temp, temp, 64);
        }
        mpz_clear(temp);
    }


void mpz_from_uint256(mpz_t dst, const uint64_t src[4]) {
        mpz_set_ui(dst, 0);
        for (int i = 3; i >= 0; i--) {
            mpz_mul_2exp(dst, dst, 64);
            mpz_add_ui(dst, dst, src[i]);
        }
    }