#include <stdint.h>
#include <stdio.h>
#include <time.h> // srand

#include "homomorph.h"


int main(int argc, char** argv) {

    (void)argc;
    (void)argv;

    const uint64_t N = 16;  // Bit count
    const pol_degree_t d = 6; // Secret key
    const pol_degree_t dp = 3; // Random element degree
    const pol_degree_t delta = 2; // Random element degree
    const uint64_t tau = 5; // Size of public key

    PubKey pk;
    Polynomial_t sk;

    srand(time(NULL));
    homomorph_keygen(d, dp, delta, tau, &pk, &sk);

    bool x = true;

    Part* part = random_part(N);

    Polynomial_t c;
    encrypt_bit(x, &pk, part, &c);

    bool y;
    decrypt_bit(&c, &sk, &y);

    printf("x = %d\n", x);
    printf("y = %d\n", y);

    return 0;
}