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
           "	Encrypts a specified file given a public key file.\n"
           "Usage\n"
           "	./encrypt [i:o:n:vh] [-i inputfile] [-o outputfile] [-n pbfile] [-v] [-h]\n"
           "Options\n"
           "	-i	Specifies input file to encrypt.\n"
           "	-o	Specifies output files to encrypt.\n"
           "	-n	Specifies the file with the public key.\n"
           "	-v	Enables verbose mode.\n"
           "	-h	Displays synopsis and usage.\n");
}

int main(int argc, char **argv) {
    int opt = 0;

    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pbfile = fopen("rsa.pub", "r");
    bool verbose_mode = false;

    while ((opt = getopt(argc, argv, "i:o:n:vh")) != -1) {
        switch (opt) {
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n':
            fclose(pbfile);
            pbfile = fopen(optarg, "r");
            break;
        case 'v': verbose_mode = true; break;
        case 'h': usage(); break;
        default: usage(); break;
        }
    }

    char username[256]; //Just chose 256 arbitrarily for user name length
    mpz_t n;
    mpz_init(n);
    mpz_t e;
    mpz_init(e);
    mpz_t s;
    mpz_init(s);

    rsa_read_pub(n, e, s, username, pbfile);

    if (verbose_mode == true) {
        printf("username = %s\n", username);
        printf("s (%lu", mpz_sizeinbase(s, 2));
        gmp_printf(" bits) = %Zd\n", s);
        printf("n (%lu", mpz_sizeinbase(n, 2));
        gmp_printf(" bits) = %Zd\n", n);
        printf("e (%lu", mpz_sizeinbase(e, 2));
        gmp_printf(" bits) = %Zd\n", e);
    }

    mpz_t user;
    mpz_init(user);
    mpz_set_str(user, username, 62);

    if (rsa_verify(user, s, e, n) == false) {
        fprintf(stderr, "Signature could not be verified.\n");
        fclose(infile);
        fclose(outfile);
        fclose(pbfile);
        mpz_clear(n);
        mpz_clear(e);
        mpz_clear(s);
        mpz_clear(user);
        return 0;
    }

    rsa_encrypt_file(infile, outfile, n, e);

    fclose(infile);
    fclose(outfile);
    fclose(pbfile);
    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(s);
    mpz_clear(user);
}
