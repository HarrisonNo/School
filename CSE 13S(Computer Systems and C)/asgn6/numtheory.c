#include "numtheory.h"
#include "randstate.h"
void gcd(mpz_t g, mpz_t a, mpz_t b) //Taken from asgn6 pdf
{
    mpz_t TempA, TempB, TempT;
    mpz_inits(TempA, TempB, TempT, NULL);
    mpz_set(TempB, b);
    mpz_set(TempA, a);
    while (mpz_cmp_d(TempB, 0) > 0) {
        mpz_set(TempT, TempB);
        mpz_mod(TempB, TempA, TempB);
        mpz_set(TempA, TempT);
    }
    mpz_set(g, TempA);
    mpz_clears(TempA, TempB, TempT, NULL);
    return; //effectively returns g
}

void mod_inverse(mpz_t o, mpz_t a, mpz_t n) //Taken from asgn6 pdf
{
    mpz_t R, RPrime, TPrime, Q, ParallelA, ParallelB;
    mpz_inits(R, RPrime, TPrime, Q, ParallelA, ParallelB, NULL);
    mpz_set(R, n);
    mpz_set(RPrime, a);
    mpz_set_si(o, 0);
    mpz_set_si(TPrime, 1);
    while (mpz_cmp_d(RPrime, 0) != 0) {
        mpz_fdiv_q(Q, R, RPrime);
        mpz_set(ParallelA, RPrime);
        mpz_mul(ParallelB, Q, RPrime);
        mpz_sub(ParallelB, R, ParallelB);
        mpz_set(R, ParallelA);
        mpz_set(RPrime, ParallelB);
        mpz_set(ParallelA, TPrime);
        mpz_mul(ParallelB, Q, TPrime);
        mpz_sub(ParallelB, o, ParallelB);
        mpz_set(o, ParallelA);
        mpz_set(TPrime, ParallelB);
    }
    if (mpz_cmp_d(R, 1) > 0) {
        mpz_set_ui(o, 0); //no inverse can be found
        mpz_clears(R, RPrime, TPrime, Q, ParallelA, ParallelB, NULL);
        return; //returns o
    }
    if (mpz_cmp_d(o, 0) < 0) {
        mpz_add(o, o, n);
    }
    mpz_clears(R, RPrime, TPrime, Q, ParallelA, ParallelB, NULL);
    return; //returns o
}

void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
    mpz_t TempO, P, TempD;
    mpz_inits(TempO, P, TempD, NULL);
    mpz_set(P, a);
    mpz_set_ui(TempO, 1);
    mpz_set(TempD, d);
    while (mpz_get_si(TempD) > 0) {
        if (mpz_odd_p(TempD)) //if odd
        {
            mpz_mul(TempO, TempO, P);
            mpz_mod(TempO, TempO, n);
        }
        mpz_mul(P, P, P);
        mpz_mod(P, P, n);
        mpz_fdiv_q_ui(TempD, TempD, 2); //should result in rounding down but we will see
    }
    mpz_set(o, TempO);
    mpz_clears(TempO, P, TempD, NULL);
    return; //returns o
}

bool is_prime(mpz_t n, uint64_t iters) {
    //Edge cases
    if (mpz_cmp_ui(n, 1) <= 0 || mpz_cmp_ui(n, 4) == 0) {
        return false;
    }
    if (mpz_cmp_ui(n, 3) <= 0) {
        return true;
    }
    //Finding d for later finding r
    mpz_t S, J, NMinusOne, SMinusOne;
    mpz_inits(S, J, NMinusOne, SMinusOne, NULL);
    mpz_set(S, n);
    mpz_sub_ui(S, S, 1);
    mpz_set(NMinusOne, S);
    while (mpz_even_p(S)) {
        mpz_fdiv_q_ui(S, S, 2);
    }
    mpz_sub_ui(SMinusOne, S, 1);
    //Iterate iters times
    mpz_t Iterator, RandomA, TempN, Y, IntHolder;
    mpz_inits(Iterator, RandomA, TempN, Y, IntHolder, NULL);
    mpz_set_si(Iterator, 1);
    mpz_set_ui(IntHolder, 2);
    mpz_set(TempN, n);
    mpz_sub_ui(TempN, TempN, 3);
    while (mpz_cmp_ui(Iterator, iters) < 0) { //MillerTest
        //Calculate random number
        mpz_urandomm(RandomA, state,
            TempN); //generates a number between 0 and n-1, need to generate a number between 2 and n-2
        mpz_add_ui(RandomA, RandomA, 2);
        //Calculate Y
        pow_mod(Y, RandomA, S, n);
        if (mpz_cmp_ui(Y, 1) != 0 && mpz_cmp(Y, NMinusOne) != 0) {
            mpz_set_ui(J, 1);
            while (mpz_cmp(J, SMinusOne) <= 0 && mpz_cmp(Y, NMinusOne) != 0) {
                pow_mod(Y, Y, IntHolder, n);
                if (mpz_cmp_ui(Y, 1) == 0) {
                    mpz_clears(
                        S, J, NMinusOne, SMinusOne, Iterator, RandomA, TempN, Y, IntHolder, NULL);
                    return false;
                }
                mpz_mul_ui(J, J,
                    2); //increment by *2 instead of +1 as +1 resulted in far too many iterations, too slow
                //technically incrementing by +1 should still work given enough time, but since the pipeline times out a program after
                //too long it would have TECHNICALLY failed
            }
            if (mpz_get_ui(Y) != mpz_get_ui(n) - 1) {
                mpz_clears(
                    S, J, NMinusOne, SMinusOne, Iterator, RandomA, TempN, Y, IntHolder, NULL);
                return false;
            }
        }
        mpz_add_ui(Iterator, Iterator, 1);
    } //End of Miller Test
    mpz_clears(S, J, NMinusOne, SMinusOne, Iterator, RandomA, TempN, Y, IntHolder, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t TempBits, MinValue, IntHolder;
    mpz_inits(TempBits, MinValue, IntHolder, NULL);
    mpz_set_ui(TempBits, bits);
    mpz_sub_ui(TempBits, TempBits, 1);
    mpz_set_ui(MinValue, 1);
    for (mpz_set_ui(IntHolder, 0); mpz_cmp(IntHolder, TempBits) < 0;
         mpz_add_ui(IntHolder, IntHolder, 1)) {
        mpz_mul_ui(MinValue, MinValue, 2); //calculates the min value which is 2^(bits-1)
    }
    do {
        mpz_urandomb(p, state, mpz_get_ui(TempBits)); //generates the random number
        mpz_add(p, p, MinValue);
    } while (!is_prime(p, iters));
    mpz_clears(TempBits, MinValue, IntHolder, NULL);
}
