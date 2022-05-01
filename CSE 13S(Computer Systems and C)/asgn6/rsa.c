#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

void logbase2(mpz_t, mpz_t);

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    mpz_t Totient, TempP, TempQ, MinNumberBits, MaxNumberBits, RandomBits, NBITS;
    mpz_inits(Totient, TempP, TempQ, MinNumberBits, MaxNumberBits, RandomBits, NBITS, NULL);
    mpz_set_ui(NBITS, nbits);
    mpz_div_ui(MinNumberBits, NBITS, 4);
    mpz_mul_ui(MaxNumberBits, NBITS, 3);
    mpz_div_ui(MaxNumberBits, MaxNumberBits, 4);
    mpz_sub(MaxNumberBits, MaxNumberBits, MinNumberBits);
    mpz_urandomm(RandomBits, state, MaxNumberBits);
    mpz_add(RandomBits, RandomBits,
        MinNumberBits); //ensures that the random number is between Min and Max NOT 0 and Max-Min
    make_prime(p, mpz_get_ui(RandomBits), iters); //generate random prime for p
    mpz_sub(RandomBits, NBITS, RandomBits); //q gets the remaining bits not used for p
    make_prime(q, mpz_get_ui(RandomBits), iters); //generate random prime for q
    //Calculating n
    mpz_mul(n, p, q);
    //Calculating totient
    mpz_sub_ui(TempP, p, 1);
    mpz_sub_ui(TempQ, q, 1);
    mpz_mul(Totient, TempP, TempQ);
    //Finding exponent e
    mpz_t RandomNum, GCD;
    mpz_inits(RandomNum, GCD, NULL);
    mpz_set_ui(GCD, 2);
    while (mpz_cmp_ui(GCD, 1) != 0) { //loop until found a coprime
        mpz_urandomb(RandomNum, state, nbits);
        gcd(GCD, RandomNum, Totient);
    }
    mpz_set(e, RandomNum);
    mpz_clears(Totient, TempP, TempQ, MinNumberBits, MaxNumberBits, RandomBits, RandomNum, GCD,
        NBITS, NULL);
    return;
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    //Prints n,e,s,username. Each with a trailing newline
    //n,e,s are hexstrings
    gmp_fprintf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n, e, s, username);
    return;
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n, e, s, username);
    return;
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t Phi, TempP, TempQ;
    mpz_inits(Phi, TempP, TempQ, NULL);
    mpz_sub_ui(TempP, p, 1);
    mpz_sub_ui(TempQ, q, 1);
    mpz_mul(Phi, TempP, TempQ); //calculate phi
    mod_inverse(d, e, Phi);
    mpz_clears(Phi, TempP, TempQ, NULL);
    return;
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    //Prints n,d. Each with a trailing newline
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", n, d);
    return;
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", n, d);
    return;
}

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    //c=m^e*modn
    pow_mod(c, m, e, n);
    return;
}

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    //Calculate block size k
    mpz_t K, BytesRead;
    mpz_inits(K, BytesRead, NULL);
    logbase2(n, K); //K=log2(n)
    mpz_sub_ui(K, K, 1);
    mpz_fdiv_q_ui(K, K, 8); //k=floor(k/8)
    //Allocate array of size K
    uint8_t *Block = malloc(mpz_get_ui(K) * sizeof(uint8_t));
    //Set 0th byte to 0xFF to bypass potential edge cases
    Block[0] = 0xFF;
    //Begin conversion of infile
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    //While not at end of file
    do {
        mpz_set_ui(BytesRead, fread(&Block[1], sizeof(uint8_t), mpz_get_ui(K) - 1, infile));
        mpz_import(m, mpz_get_ui(BytesRead) + 1, 1, sizeof(uint8_t), 1, 0,
            Block); //bytesread+1 to include 0xFF
        rsa_encrypt(c, m, e, n);
        gmp_fprintf(outfile, "%Zx\n", c);
    } while (mpz_cmp_ui(BytesRead, EOF) != 0 && mpz_cmp_ui(BytesRead, 0) != 0);
    free(Block);
    mpz_clears(K, BytesRead, m, c, NULL);
    return;
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    //m=c^d*modn
    pow_mod(m, c, d, n);
    return;
}

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    //Calculate block size k
    mpz_t K, m, c, BytesRead;
    mpz_inits(K, m, c, BytesRead, NULL);
    size_t BytesConverted;
    logbase2(n, K); //count=log2(n)
    mpz_sub_ui(K, K, 1);
    mpz_fdiv_q_ui(K, K, 8); //k=floor(k/8)
    //Allocate array of size K
    uint8_t *Block = malloc(mpz_get_ui(K) * sizeof(uint8_t));
    //Begin conversion of infile
    //While not at end of file
    do {
        mpz_set_ui(BytesRead, gmp_fscanf(infile, "%Zx\n", c));
        rsa_decrypt(m, c, d, n);
        mpz_export(Block, &BytesConverted, 1, sizeof(uint8_t), 1, 0, m);
        fwrite(&Block[1], sizeof(uint8_t), BytesConverted - 1, outfile);
    } while (mpz_cmp_ui(BytesRead, EOF) != 0 && mpz_cmp_ui(BytesRead, 0) != 0);
    mpz_clears(K, c, m, BytesRead, NULL);
    free(Block);
    return;
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    //s=m^d*modn
    pow_mod(s, m, d, n);
    return;
}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    //t=s^e*modn
    //Only verified if t is the same as message m
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n);
    if (mpz_cmp(t, m) == 0) {
        mpz_clear(t);
        return true;
    }
    mpz_clear(t);
    return false;
}

void logbase2(mpz_t n, mpz_t count) //Credit to Elmer on the CSE13s discord for the python code
{
    mpz_t TempN;
    mpz_init(TempN);
    mpz_set(TempN, n);
    mpz_set_ui(count, 1);
    while (mpz_get_ui(TempN) > 0) {
        mpz_fdiv_q_ui(TempN, TempN, 2); //floor(divide by 2)
        mpz_add_ui(count, count, 1);
    }
    mpz_clear(TempN);
    return; //effectively returns the count
}
