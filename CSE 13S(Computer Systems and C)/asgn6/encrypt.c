#include <unistd.h>
#include <stdlib.h>
#include "rsa.h"

#define OPTIONS "i:o:n:vh"

void PrintHelpPage();

int main(int NumberOfArgs, char *Strings[]) {
    int opt = 0;
    bool verbose = false;
    char Username[100]; //max username size of 100, shouldnt matter though
    FILE *InputFile = stdin, *OutputFile = stdout, *RSAFile = NULL;
    //Parsing command line options
    while ((opt = getopt(NumberOfArgs, Strings, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': //Prints out a help message describing the purpose of the program and the command-line op-tions it accepts, exiting the program afterwards.
            PrintHelpPage();
            return 0;
        case 'i': //specifies the input file to encrypt (default: stdin).
            InputFile = fopen(optarg, "r");
            if (!InputFile) {
                fprintf(stdout, "Error: failed to open infile");
                return 0;
            }
            break;
        case 'o': //specifies the output file to decrypt (default: stdout).
            OutputFile = fopen(optarg, "w");
            if (!OutputFile) {
                fprintf(stdout, "Error: failed to open outfile");
                return 0;
            }
            break;
        case 'v': //enables verbose output.
            verbose = true;
            break;
        case 'n': //specifies the file containing the public key (default: rsa.pub).
            RSAFile = fopen(optarg, "r");
            if (!RSAFile) {
                fprintf(stdout, "Error: failed to open RSA file");
                return 0;
            }
            break;
        }
    }
    //Opening files
    if (RSAFile == NULL) {
        RSAFile = fopen("rsa.pub", "r");
        if (!RSAFile) {
            fprintf(stdout, "Error: failed to open RSA file");
            return 0;
        }
    }
    //Read key from public file
    mpz_t n, e, s;
    mpz_inits(n, e, s, NULL);
    rsa_read_pub(n, e, s, &Username[0], RSAFile);
    //verbose output
    if (verbose) {
        printf("user = %s\n", Username);
        gmp_printf("s (%dbits) = %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_printf("n (%dbits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%dbits) = %Zd\n", mpz_sizeinbase(e, 2), e);
    }
    //Converting and verifying username
    mpz_t ConvertedUsername;
    mpz_init(ConvertedUsername);
    mpz_set_str(ConvertedUsername, Username, 62); //Converts in base 62
    if (!rsa_verify(ConvertedUsername, s, e, n)) {
        printf("Failed to verify username, exiting program\n");
        return 0;
    }
    //Encrypting file
    rsa_encrypt_file(InputFile, OutputFile, n, e);
    //Exiting and clearing
    if (InputFile != stdin) {
        fclose(InputFile);
    }
    if (OutputFile != stdout) {
        fclose(OutputFile);
    }
    fclose(RSAFile);
    mpz_clears(n, e, s, ConvertedUsername, NULL);
    return 0;
}

void PrintHelpPage() {
    printf("SYNOPSIS\n");
    printf("\tEncrypts data using RSA encryption.\n");
    printf("\tEncrypted data is decrypted by the decrypt program.\n\n");
    printf("USAGE\n");
    printf("\t./encrypt [-hv] [-i infile] [-o outfile] -n pubkey -d privkey\n\n");
    printf("OPTIONS\n");
    printf("\t-h\t\tDisplay program help and usage.\n");
    printf("\t-v\t\tDisplay verbose program output.\n");
    printf("\t-i infile\tInput file of data to encrypt (default: stdin).\n");
    printf("\t-o outfile\tOutput file for encrypted data (default: stdout).\n");
    printf("\t-n pbfile\tPublic key file (default: rsa.pub).\n");
    return;
}
