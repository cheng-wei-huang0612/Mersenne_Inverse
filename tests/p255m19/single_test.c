#include <stdint.h>
#include <stdio.h>
#include <gmp.h>


extern void uint256_from_mpz(uint64_t dst[4], const mpz_t src);
extern void cpt_inv(uint64_t *x, uint64_t *inv);
extern void mpz_from_uint256(mpz_t dst, const uint64_t src[4]);

// const char *TEST_X = "97165018423823247783865467928919784927142320732975169496993440879442771425739";
// const char *TEST_X = "29117641286608306238495715644116414192630722323124200381980140954205159429133";
// const char *TEST_X = "51594937704745900782878427907903488982031175296730183711367891400868856617456";
// const char *TEST_X = "8447652333555256815180248154595013921330191454237389748465485098126138843358";
const char *TEST_X = "41539168527567215076744596298652135611477756802866708682274748875823690114131";


int main(void) {

    // Initialize GMP variables for prime P (in mpz form).
    mpz_t mpP;
    mpz_init(mpP);
    mpz_set_ui(mpP, 1);
    mpz_mul_2exp(mpP, mpP, 255); // P = 2^255
    mpz_sub_ui(mpP, mpP, 19); // P = 2^255 - 19


    mpz_t mpX;
    mpz_init(mpX);
    mpz_set_str(mpX, TEST_X, 10);
    uint64_t x[4];

    uint256_from_mpz(x, mpX);


    uint64_t xInv[4] = {0};
    cpt_inv(x, xInv);

    mpz_t mpInv;
    mpz_init(mpInv);

    mpz_from_uint256(mpInv, xInv);

    mpz_t mpInvRef;
mpz_init(mpInvRef);

/* mpz_invert() 回傳 0 代表 gcd(x,P) ≠ 1，理論上對 Ed25519 不會發生 */
if (!mpz_invert(mpInvRef, mpX, mpP)) {
    fprintf(stderr, "x 與 P 不互質，無法取反元素！\n");
    return 1;
}

if (mpz_cmp(mpInv, mpInvRef) == 0) {
    /* ---------- Case 1：完美符合 ---------- */
    gmp_printf(
        "Case 1 ‧ 完全符合標準代表\n"
        "  x        = %Zd\n"
        "  inv(x)   = %Zd\n",      /* 你的結果 */
        mpX, mpInv
    );

} else {
    /* 檢查兩者差是否為 P 的倍數 */
    mpz_t diff;
    mpz_init(diff);
    mpz_sub(diff, mpInv, mpInvRef);   /* 可能為負值 */
    mpz_mod(diff, diff, mpP);         /* 取 (inv - invRef) mod P */

    if (mpz_cmp_ui(diff, 0) == 0) {
        /* ------ Case 2：同一類別，但代表元不同 ------ */
        gmp_printf(
            "Case 2 ‧ 成功計算反元素，但非 GMP 標準代表\n"
            "  x          = %Zd\n"
            "  your inv   = %Zd\n"
            "  ref inv    = %Zd (GMP)\n",
            mpX, mpInv, mpInvRef
        );
    } else {
        /* -------------- Case 3：錯誤 -------------- */
        gmp_printf(
            "Case 3 ‧ 反元素計算錯誤！\n"
            "  x          = %Zd\n"
            "  your inv   = %Zd\n"
            "  ref inv    = %Zd (GMP)\n",
            mpX, mpInv, mpInvRef
        );
        mpz_clear(diff);
        mpz_clear(mpInvRef);
        return 1;
    }
    mpz_clear(diff);
}

mpz_clear(mpInvRef);
}

