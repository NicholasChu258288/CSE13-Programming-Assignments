#include "numtheory.h"
#include "randstate.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <gmp.h>

void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    mpz_t e;
    mpz_init_set(e, exponent);
    mpz_t b;
    mpz_init_set(b, base); //b = base
    mpz_set_ui(out, 1); //out = 1
    while (mpz_cmp_ui(e, 0) > 0) { // while e > 0
        if (mpz_odd_p(e)) { // if e is odd
            mpz_mul(out, out, b); //out = out * b
            mpz_mod(out, out, modulus); //out = out mod modulus
        }
        mpz_mul(b, b, b); //b = b*b
        mpz_mod(b, b, modulus); //b = b mod modulus
        mpz_fdiv_q_ui(e, e, 2);
    }
    //Clearing mpz's used for function
    mpz_clear(e);
    mpz_clear(b);
}

bool is_prime(mpz_t n, uint64_t iters) {
    //Special cases for when n is 1 through 4
    //This is to avoid issues when modding n-4 later
    //since n-4 cant needs to be above 4 to work properly
    if (mpz_cmp_ui(n, 1) == 0 || mpz_cmp_ui(n, 4) == 0) {
        return false;
    }
    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) {
        return true;
    }

    mpz_t r;
    mpz_init_set(r, n); // r = n
    mpz_t s;
    mpz_init(s); // s = 0
    //Writing n-1 = 2^s * r so r is odd
    mpz_sub_ui(r, r, 1); // r = r - 1
    while (mpz_even_p(r) && mpz_cmp_ui(r, 0) != 0) {
        mpz_tdiv_q_ui(r, r, 2); //r = r/2
        mpz_add_ui(s, s, 1); //s += 1
    }

    mpz_t y;
    mpz_init(y);
    mpz_t a; // For random number
    mpz_init(a);
    mpz_t mod; // Used for getting rand num in range
    mpz_init_set(mod, n);
    mpz_sub_ui(mod, mod, 3); // mod = n-3
    mpz_t exponent;
    mpz_init_set_ui(exponent, 2); // expoenent = 2
    //uint64_t n_minus_1 = mpz_get_ui(n) - 1;
    mpz_t n_minus_1;
    mpz_init_set(n_minus_1, n);
    mpz_sub_ui(n_minus_1, n_minus_1, 1); // n_minus_1 = n - 1
    mpz_t s_minus_1;
    mpz_init_set(s_minus_1, s);
    mpz_sub_ui(s_minus_1, s_minus_1, 1); // s_minus_1 = s - 1
    mpz_t j;
    mpz_init(j);

    for (uint64_t i = 1; i < iters; i += 1) {
        //Choose random num from range [2,n-2]
        //To to this: rand % (n-3) + 2
        mpz_urandomm(a, state, n); // a = random number from [0,n-1]
        mpz_mod(a, a, mod); // a = a mod (n-3)
        mpz_add_ui(a, a, 2); // a = a + 1
        pow_mod(y, a, r, n); // y = a ^ r mod n
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp_si(y, mpz_get_si(n_minus_1)) != 0) {
            mpz_set_ui(j, 1);
            while (mpz_cmp_si(j, mpz_get_si(s_minus_1)) <= 0 && mpz_cmp(y, n_minus_1) != 0) {
                pow_mod(y, y, exponent, n);
                if (mpz_cmp_ui(y, 1) == 0) {
                    //Cleaning and returning
                    mpz_clear(r);
                    mpz_clear(s);
                    mpz_clear(y);
                    mpz_clear(a);
                    mpz_clear(exponent);
                    mpz_clear(mod);
                    mpz_clear(n_minus_1);
                    mpz_clear(s_minus_1);
                    mpz_clear(j);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, n_minus_1) != 0) {
                //Cleaning and returning
                mpz_clear(r);
                mpz_clear(s);
                mpz_clear(y);
                mpz_clear(a);
                mpz_clear(exponent);
                mpz_clear(mod);
                mpz_clear(n_minus_1);
                mpz_clear(s_minus_1);
                mpz_clear(j);
                return false;
            }
        }
    }
    //Cleaning and returning true
    mpz_clear(r);
    mpz_clear(s);
    mpz_clear(y);
    mpz_clear(a);
    mpz_clear(exponent);
    mpz_clear(mod);
    mpz_clear(n_minus_1);
    mpz_clear(s_minus_1);
    mpz_clear(j);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t bit_size;
    mpz_init(bit_size);
    mpz_ui_pow_ui(bit_size, 2, bits);
    //Generates random number of at least size bits from range [0, 2^bits]
    mpz_urandomb(p, state, bits);
    mpz_add(p, p, bit_size); //Adding 2^n bits to make sure min values are larger than 2^bits
    //Continues to generate numbers until a prime is found
    while (is_prime(p, iters) == false) {
        mpz_urandomb(p, state, bits);
        mpz_add(p, p, bit_size);
    }
    mpz_clear(bit_size);
}

void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_set(d, a);
    mpz_t B;
    mpz_init_set(B, b);
    mpz_t tmp;
    mpz_init(tmp);
    while (mpz_cmp_ui(B, 0) != 0) { //While b != 0
        mpz_set(tmp, B); // tmp = b
        mpz_set(B, d); // b = d
        mpz_mod(B, B, tmp); // b = b mod tmp
        mpz_set(d, tmp); //d = tmp
    }
    //Cleaning
    mpz_clear(B);
    mpz_clear(tmp);
}

void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    mpz_t r;
    mpz_init_set(r, n); // r = n
    mpz_t r_prime;
    mpz_init_set(r_prime, a); // r' = a
    mpz_t t;
    mpz_init_set_ui(t, 0); // t = 0
    mpz_t t_prime;
    mpz_init_set_ui(t_prime, 1); // t' = 1
    mpz_t q;
    mpz_init(q); //q = 0
    mpz_t temp;
    mpz_init(temp); //tmp = 0
    mpz_t product;
    mpz_init(product);

    while (mpz_cmp_ui(r_prime, 0) != 0) {
        mpz_fdiv_q(q, r, r_prime); //q = r floor div r'
        mpz_set(temp, r); //tmp = r
        mpz_set(r, r_prime); //r = r'
        mpz_mul(product, q, r_prime); // product = q * r'
        mpz_sub(r_prime, temp, product); // r' = r - product
        mpz_set(temp, t); // i = tmp
        mpz_set(t, t_prime); // t = t'
        mpz_mul(product, q, t_prime); //product = q * t'
        mpz_sub(t_prime, temp, product);
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(i, 0);
        mpz_clear(r);
        mpz_clear(r_prime);
        mpz_clear(t);
        mpz_clear(t_prime);
        mpz_clear(q);
        mpz_clear(temp);
        mpz_clear(product);
        return;
    }
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }
    mpz_set(i, t);
    mpz_clear(r);
    mpz_clear(r_prime);
    mpz_clear(t);
    mpz_clear(t_prime);
    mpz_clear(q);
    mpz_clear(temp);
    mpz_clear(product);
    return;
}
