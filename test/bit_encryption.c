#include <stdint.h>
#include <stdio.h>
#include <time.h> // srand

#include "homomorph.h"


int main(int argc, char** argv) {

    (void)argc;
    (void)argv;

    // const uint64_t N = 16;  // Bit count
    const pol_degree_t d = 6; // Secret key
    const pol_degree_t dp = 3; // Random element degree
    const pol_degree_t delta = 2; // Random element degree
    const uint64_t tau = 5; // Size of public key

    srand(time(NULL));

    static const uint16_t nb_test = 10;

    bool x, y;
    Polynomial_t c;
    HomomContext ctx = {0};
    for (uint16_t i = 0; i < nb_test; i++) {
        homomorph_init(d, dp, delta, tau, &ctx);

        x = rand()%2;

        Part part = random_part(tau);

        encrypt_bit(x, ctx.pk, part, &c);

        decrypt_bit(c, ctx.sk, &y);

        if (x != y) {
            printf("> ERROR x != y\n");
        } else {
            printf("Test passed.\n");
        }

        delete_part(part);
        delete_polynom(c);
        homomorph_clear(ctx);
    }

    return 0;
}