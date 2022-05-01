#include <unistd.h>
#include <stdlib.h>
#include "rsa.h"

#define OPTIONS "i:o:n:vh"

void PrintHelpPage();

int main(int NumberOfArgs, char *Strings[]) {
    int opt = 0;
    bool verbose = false;
    FILE *InputFile = stdin, *OutputFile = stdout, *RSAFile = NULL;
    //Parsing command line options
    while ((opt = getopt(NumberOfArgs, Strings, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': //Prints out a help message describing the purpose of the program and the command-line op-tions it accepts, exiting the program afterwards.
            PrintHelpPage();
            return 0;
        case 'i': //specifies the input file to decrypt (default: stdin).
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
        case 'n': //specifies the file containing the private key (default: rsa.priv).
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
        RSAFile = fopen("rsa.priv", "r");
        if (!RSAFile) {
            fprintf(stdout, "Error: failed to open RSA file");
            if (!RSAFile) {
                fprintf(stdout, "Error: failed to open RSA file");
                return 0;
            }
            return 0;
        }
    }
    //Read key from private file
    mpz_t n, d;
    mpz_inits(n, d, NULL);
    rsa_read_priv(n, d, RSAFile);
    //verbose output
    if (verbose) {
        gmp_printf("n (%dbits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("d (%dbits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }
    //Encrypting file
    rsa_decrypt_file(InputFile, OutputFile, n, d);
    //Exiting and clearing
    if (InputFile != stdin) {
        fclose(InputFile);
    }
    if (OutputFile != stdout) {
        fclose(OutputFile);
    }
    fclose(RSAFile);
    mpz_clears(n, d, NULL);
    return 0;
}

void PrintHelpPage() {
    printf("SYNOPSIS\n");
    printf("\tDecrypts data using RSA decryption.\n");
    printf("\tDecrypted data is encrypted by the encrypt program.\n\n");
    printf("USAGE\n");
    printf("\t./decrypt [-hv] [-i infile] [-o outfile] -n pubkey -d privkey\n\n");
    printf("OPTIONS\n");
    printf("\t-h\t\tDisplay program help and usage.\n");
    printf("\t-v\t\tDisplay verbose program output.\n");
    printf("\t-i infile\tInput file of data to decrypt (default: stdin).\n");
    printf("\t-o outfile\tOutput file for decrypted data (default: stdout).\n");
    printf("\t-n pbfile\tPrivate key file (default: rsa.priv).\n");
    return;
}
