#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <gmp.h>

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    //Making primes
    uint64_t nbits_p
        = random() % (nbits / 2 + 1) + (nbits / 4); // rand number in range of [nbits/4,3*nbits/4)
    uint64_t nbits_q = nbits - nbits_p; //Remaining bits for used for q
    make_prime(p, nbits_p, iters); //Make prime p
    make_prime(q, nbits_q, iters); //Make prime q
    mpz_mul(n, p, q); // n = p * q
    // Generating primes until log2(n) >= nbits
    if (nbits > mpz_sizeinbase(n, 2)) {
        make_prime(p, nbits_p, iters); //Make prime p
        make_prime(q, nbits_q, iters); //Make prime q
        mpz_mul(n, p, q); // n = p * q
    }

    //Finding lambda n
    mpz_t p_minus_1;
    mpz_init(p_minus_1);
    mpz_sub_ui(p_minus_1, p, 1);
    mpz_t q_minus_1;
    mpz_init(q_minus_1);
    mpz_sub_ui(q_minus_1, q, 1);
    mpz_t lambda; //Used for holding lambda(n)
    mpz_init(lambda);
    mpz_t numer;
    mpz_init(numer);
    mpz_mul(numer, p_minus_1, q_minus_1); // numer = (p-1) * (q-1)
    mpz_abs(numer, numer); //numer = |(p-1) * (q-1)|
    mpz_t denom;
    mpz_init(denom);
    gcd(denom, p_minus_1, q_minus_1);
    mpz_divexact(lambda, numer, denom);

    mpz_urandomb(e, state, nbits);

    mpz_t e_gcd;
    mpz_init(e_gcd);
    gcd(e_gcd, e, lambda);

    //Finding appropriate e
    while (mpz_cmp_ui(e_gcd, 1) != 0) { //While e_gcd != 0
        mpz_urandomb(e, state, nbits);
        gcd(e_gcd, e, lambda);
    }

    //Cleaning
    mpz_clear(p_minus_1);
    mpz_clear(q_minus_1);
    mpz_clear(numer);
    mpz_clear(denom);
    mpz_clear(lambda);
    mpz_clear(e_gcd);
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n); //Prints in hex
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    fprintf(pbfile, "%s\n", username);
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n); //Scans hex
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    fscanf(pbfile, "%s\n", username);
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    //Finding lambda(n)
    mpz_t p_minus_1;
    mpz_init(p_minus_1);
    mpz_sub_ui(p_minus_1, p, 1);
    mpz_t q_minus_1;
    mpz_init(q_minus_1);
    mpz_sub_ui(q_minus_1, q, 1);
    mpz_t lambda; //Used for holding lambda(n)
    mpz_init(lambda);
    mpz_t numer;
    mpz_init(numer);
    mpz_mul(numer, p_minus_1, q_minus_1);
    mpz_abs(numer, numer); //numer = |(p-1) * (q-1)|
    mpz_t denom;
    mpz_init(denom);
    gcd(denom, p_minus_1, q_minus_1); //denom = gcd(p-1,q-1)
    mpz_divexact(lambda, numer, denom);
    //Mod inverse of e lambda(n)
    mod_inverse(d, e, lambda);

    //Cleaning
    mpz_clear(p_minus_1);
    mpz_clear(q_minus_1);
    mpz_clear(numer);
    mpz_clear(denom);
    mpz_clear(lambda);
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);
}

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
}

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    mpz_t k; //Block size
    mpz_init(k);
    //Gets a little over log2(n) (Might need to adjust by subtracting 1)
    mpz_set_ui(k, mpz_sizeinbase(n, 2));
    mpz_sub_ui(k, k, 1);
    mpz_fdiv_q_ui(k, k, 8); // k = (log2(n)-1)/8

    mpz_t c;
    mpz_init(c);
    mpz_t m;
    mpz_init(m);

    uint8_t *block = calloc(mpz_get_ui(k), sizeof(uint8_t));
    block[0] = 0xFF;
    size_t j = 0;

    j = fread(&block[1], sizeof(uint8_t), mpz_get_ui(k), infile);
    mpz_import(m, j, 1, sizeof(uint8_t), 1, 0, block); //Saving block to c
    rsa_encrypt(c, m, e, n);
    gmp_fprintf(outfile, "%Zx\n", c);

    //Going through and excrypting by blocks
    while (j = fread(&block[1], sizeof(uint8_t), mpz_get_ui(k), infile), j > 0) {
        mpz_import(m, j, 1, sizeof(uint8_t), 1, 0, block);
        rsa_encrypt(c, m, e, n);
        gmp_fprintf(outfile, "%Zx\n", c);
    }

    //Cleaning
    mpz_clear(k);
    mpz_clear(c);
    mpz_clear(m);
    free(block);
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
}

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    mpz_t k; //Block size
    mpz_init(k);
    //Finding block size
    mpz_set_ui(k, mpz_sizeinbase(n, 2));
    mpz_sub_ui(k, k, 1);
    mpz_fdiv_q_ui(k, k, 8); // k = (log2(n)-1)/8
    //mpz's used for decryption loop

    mpz_t c;
    mpz_init(c);
    mpz_t m;
    mpz_init(m);
    mpz_t bytes_scanned;
    mpz_init(bytes_scanned);

    uint8_t *block = calloc(mpz_get_ui(k), sizeof(uint8_t));
    size_t j = 0;

    while (gmp_fscanf(infile, "%Zx\n", c) != EOF) {
        mpz_set_ui(bytes_scanned, mpz_sizeinbase(c, 2));
        mpz_sub_ui(bytes_scanned, bytes_scanned, 1);
        mpz_fdiv_q_ui(bytes_scanned, bytes_scanned, 8);
        j = mpz_get_ui(bytes_scanned);
        rsa_decrypt(m, c, d, n);
        mpz_export(block, &j, 1, sizeof(uint8_t), 1, 0, m);
        fwrite(&block[1], sizeof(char), sizeof(uint8_t) * (j - 1), outfile);
    }
    //Cleaning
    mpz_clear(k);
    mpz_clear(c);
    mpz_clear(m);
    mpz_clear(bytes_scanned);
    free(block);
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    //Signs s
    pow_mod(s, m, d, n);
}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    //Verifies signature by checking if m = s^e (mod n)
    mpz_t v;
    mpz_init(v);
    pow_mod(v, s, e, n);
    if (mpz_cmp(v, m) == 0) {
        mpz_clear(v);
        return true;
    } else {
        mpz_clear(v);
        return false;
    }
}
