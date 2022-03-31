#include "rsa.h"
#include "randstate.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <gmp.h>

void usage() {
    printf("Synopsis\n"
           "    Decrypts a specified file given a private key file.\n"
           "Usage\n"
           "    ./decrypt [i:o:n:vh] [-i inputfile] [-o outputfile] [-n pbfile] [-v] [-h]\n"
           "Options\n"
           "    -i      Specifies input file to decrypt.\n"
           "    -o      Specifies output files to decrypt.\n"
           "    -n      Specifies the file with the private key.\n"
           "    -v      Enables verbose mode.\n"
           "    -h      Displays synopsis and usage.\n");
}

int main(int argc, char **argv) {
    int opt = 0;

    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pvfile = fopen("rsa.priv", "r");
    bool verbose_mode = false;

    while ((opt = getopt(argc, argv, "i:o:n:vh")) != -1) {
        switch (opt) {
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n':
            fclose(pvfile);
            pvfile = fopen(optarg, "r");
            break;
        case 'v': verbose_mode = true; break;
        case 'h': usage(); break;
        default: usage(); break;
        }
    }

    mpz_t n;
    mpz_init(n);
    mpz_t d;
    mpz_init(d);

    rsa_read_priv(n, d, pvfile);

    if (verbose_mode == true) {
        printf("n (%lu", mpz_sizeinbase(n, 2));
        gmp_printf(" bits) = %Zd\n", n);
        printf("d (%lu", mpz_sizeinbase(d, 2));
        gmp_printf(" bits) = %Zd\n", d);
    }

    rsa_decrypt_file(infile, outfile, n, d);

    fclose(infile);
    fclose(outfile);
    fclose(pvfile);
    mpz_clear(n);
    mpz_clear(d);
}
