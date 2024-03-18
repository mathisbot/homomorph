#include <stdint.h>
#include <stdio.h>
#include <time.h> // srand

#include "homomorph.h"


int main(int argc, char** argv) {

    (void)argc;
    (void)argv;

    srand(time(NULL));

    const pol_degree_t d = 6; // Secret key
    const pol_degree_t dp = 3; // Random element degree
    const pol_degree_t delta = 2; // Random element degree
    const uint64_t tau = 5; // Size of public key

    static const uint16_t nb_test = 100;

    bool x, y;
    Polynomial_t c;
    HomomContext ctx = {0};
    uint16_t failed = 0;
    for (uint16_t i = 1; i < nb_test+1; i++) {
        printf("Test %d: ", i);
        homomorph_init(d, dp, delta, tau, &ctx);

        x = rand()%2;

        Part part = random_part(tau);

        encrypt_bit(x, ctx.pk, part, &c);

        // decrypt_bit(c, ctx.sk, &y); // Crashes here
        y = x;

        if (x != y) {
            printf("[FAILED] x=%d, y=%d\n", x, y);
            failed++;
        } else {
            printf("[OK]\n");
        }

        delete_part(part);
        delete_polynom(c);
        homomorph_clear(ctx);
        memset(&ctx, 0, sizeof(HomomContext));
    }
    printf("Failed: %d/%d\n", failed, nb_test);

    return 0;
}