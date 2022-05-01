#include "rsa.h"
#include "randstate.h"
#include <time.h> //needed for time()
#include <sys/stat.h> //needed for fchmod
#include <unistd.h>
#include <stdlib.h>

#define OPTIONS "b:i:n:d:s:vh"

void PrintHelpPage();

int main(int NumberOfArgs, char *Strings[]) {
    int opt = 0, PubFileInt, PrivFileInt;
    uint64_t iterations = 50, bits = 256, seed = time(NULL);
    bool verbose = false;
    char *PublicFileName = "rsa.pub", *PrivateFileName = "rsa.priv", *UserName;
    FILE *PublicFile, *PrivateFile;
    //Parsing command line options
    while ((opt = getopt(NumberOfArgs, Strings, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': //Prints out a help message describing the purpose of the program and the command-line op-tions it accepts, exiting the program afterwards.
            PrintHelpPage();
            return 0;
        case 'i': //specifies the number of Miller-Rabin iterations for testing primes (default: 50).
            iterations = atoi(optarg);
            break;
        case 'b': //specifies the minimum bits needed for the public modulus n.
            bits = atoi(optarg);
            break;
        case 'v': //enables verbose output.
            verbose = true;
            break;
        case 'n': PublicFileName = optarg; break;
        case 'd': PrivateFileName = optarg; break;
        case 's': seed = atoi(optarg); break;
        }
    }
    //Opening files
    PublicFile = fopen(PublicFileName, "w");
    PrivateFile = fopen(PrivateFileName, "w");
    if (!PublicFile || !PrivateFile) {
        printf("Failed to open desired files for printing keys, exiting program.\n");
        return 0;
    }
    //Change file permissions
    PubFileInt = fileno(PublicFile);
    PrivFileInt = fileno(PrivateFile);
    fchmod(PubFileInt, 0600);
    fchmod(PrivFileInt, 0600);
    //Initialize random state
    randstate_init(seed);
    //Make public and private keys
    mpz_t p, q, n, e, d, NameMPZ, s;
    mpz_inits(p, q, n, e, d, NameMPZ, s, NULL);
    rsa_make_pub(p, q, n, e, bits, iterations);
    rsa_make_priv(d, e, p, q);
    //Get users name and convert to mpz and calculate the signature
    UserName = getenv("USER");
    mpz_set_str(NameMPZ, UserName, 62);
    rsa_sign(s, NameMPZ, d, n);
    //Writing pub and priv keys to their files
    rsa_write_pub(n, e, s, UserName, PublicFile);
    rsa_write_priv(n, d, PrivateFile);
    //Verbose output
    if (verbose) {
        printf("user = %s\n", UserName);
        gmp_printf("s (%dbits) = %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_printf("p (%dbits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q (%dbits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n (%dbits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%dbits) = %Zd\n", mpz_sizeinbase(e, 2), e);
        gmp_printf("d (%dbits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }
    //Clearing
    fclose(PublicFile);
    fclose(PrivateFile);
    mpz_clears(p, q, n, e, d, NameMPZ, s, NULL);
    randstate_clear();
    return 0;
}

void PrintHelpPage() {
    printf("SYNOPSIS\n");
    printf("\tGenerates an RSA public/private key pair.\n\n");
    printf("USAGE\n");
    printf("\t./keygen [-hv] [-b bits] -n pbfile -d pvfile\n\n");
    printf("OPTIONS\n");
    printf("\t-h\t\tDisplay program help and usage.\n");
    printf("\t-v\t\tDisplay verbose program output.\n");
    printf("\t-b bits\tMinimum bits needed for public key n.\n");
    printf("\t-c\tMiller-Rabin iterations for testing primes (default: 50).\n");
    printf("\t-n\tPublic key file (default: rsa.pub).\n");
    printf("\t-d\tPrivate key file (default: rsa.priv).\n");
    printf("\t-s\tRandom seed for testing.\n");
    return;
}
