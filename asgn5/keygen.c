#include "rsa.h"
#include "randstate.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <gmp.h>

void usage() {
    printf("Synopsis\n"
           "Generates key pairs that will be used for RSA encryption and decryption.\n"
           "Usage\n"
           "	./keygen [b:i:n:d:s:vh] [-b n] [-i iters] [-n pbfile] [-d pvfile] [s seed] [-v] "
           "[-h]\n"
           "Options\n"
           "	-b n		Specifies number of minimum bits used for public modulus.\n"
           "	-i iters	Specifies number of iteratons for primality testing. \n"
           "	-n pbfile	Specifies file for public key.\n"
           "	-d pvfile	Specifies file for private key.\n"
           "	-s seed		Specifies the specific seed used for seeding random state.\n"
           "	-v		Enables verbose mode.\n"
           "	-h		Displays usage.\n");
}

int main(int argc, char **argv) {
    int opt = 0;

    //Setting up default cases
    uint64_t nbits = 256;
    uint64_t iters = 50;
    bool seed_specified = false;
    uint64_t seed;
    FILE *pbfile;
    pbfile = fopen("rsa.pub", "w");
    FILE *pvfile;
    pvfile = fopen("rsa.priv", "w");
    bool verbose_mode = false;

    //Reading command line input
    while ((opt = getopt(argc, argv, "b:i:n:d:s:vh")) != -1) {
        switch (opt) {
        case 'b': nbits = atoi(optarg); break;
        case 'i': iters = atoi(optarg); break;
        case 'n':
            fclose(pbfile);
            pbfile = fopen(optarg, "w");
            break;
        case 'd':
            fclose(pvfile);
            pvfile = fopen(optarg, "w");
            break;
        case 's':
            seed = atoi(optarg);
            seed_specified = true;
            break;
        case 'v': verbose_mode = true; break;
        case 'h': usage(); break;
        default: usage(); break;
        }
    }

    //Set file permissions
    fchmod(fileno(pvfile), 0600);

    //Initializes random state
    if (seed_specified == true) {
        randstate_init(seed);
    } else {
        randstate_init(time(NULL));
    }

    mpz_t p;
    mpz_init(p);
    mpz_t q;
    mpz_init(q);
    mpz_t n;
    mpz_init(n);
    mpz_t e;
    mpz_init(e);
    mpz_t d;
    mpz_init(d);

    //Making keys
    rsa_make_pub(p, q, n, e, nbits, iters);
    rsa_make_priv(d, e, p, q);

    //Signing username
    char *username = getenv("USER");
    mpz_t user;
    mpz_init(user);
    mpz_set_str(user, username, 62);
    mpz_t s;
    mpz_init(s);
    rsa_sign(s, user, d, n);

    //Writing to appropriate files
    rsa_write_pub(n, e, s, username, pbfile);
    rsa_write_priv(n, d, pvfile);

    //Printing if verbose
    if (verbose_mode == true) {
        printf("user = %s\n", username);
        printf("s (%lu", mpz_sizeinbase(n, 2));
        gmp_printf(" bits) = %Zd\n", s);
        printf("p (%lu", mpz_sizeinbase(p, 2));
        gmp_printf(" bits) = %Zd\n", p);
        printf("q (%lu", mpz_sizeinbase(q, 2));
        gmp_printf(" bits) = %Zd\n", q);
        printf("n (%lu", mpz_sizeinbase(n, 2));
        gmp_printf(" bits) = %Zd\n", n);
        printf("e (%lu", mpz_sizeinbase(e, 2));
        gmp_printf(" bits) = %Zd\n", e);
        printf("d (%lu", mpz_sizeinbase(d, 2));
        gmp_printf(" bits) = %Zd\n", d);
    }

    //Cleaning
    randstate_clear();
    fclose(pbfile);
    fclose(pvfile);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(d);
    mpz_clear(user);
    mpz_clear(s);

    return 0;
}
