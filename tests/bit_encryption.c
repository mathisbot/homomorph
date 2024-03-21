#include <stdint.h>
#include <stdio.h>
#include <time.h> // srand
#include <assert.h>

#include "utils.h"
#include "polynom.h"
#include "homomorph.h"


int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    srand(time(NULL));

    const pol_degree_t d = 10; // Secret key
    const pol_degree_t dp = 7; // Random element degree
    const pol_degree_t delta = 3; // Random element degree
    const uint64_t tau = 10; // Size of public key

    static const uint16_t nb_test = 100;

    /* --- Test Part ---*/
    printf("Part test\n");
    Part part;

    // Test random_part
    part = random_part(tau);
    assert(part.size == tau);
    assert(part.elements != NULL);
    delete_part(part);
    printf(" > random_part test passed\n");

    printf("Part test passed\n");

    /* --- Test Polynomial ---*/
    printf("Polynomial test\n");
    Polynomial_t p, p1, p2, p3;

    // Test monom
    p = monom(d);
    assert(p.degree == d);
    assert(p.coefficients != NULL);
    delete_polynom(p);
    printf(" > monom test passed\n");

    // Test constant_polynom
    p = constant_polynom(true);
    assert(p.degree == 0);
    assert(p.coefficients != NULL);
    delete_polynom(p);
    printf(" > constant_polynom test passed\n");

    // Test random_polynom
    p = random_polynom(d);
    assert(p.degree == d);
    assert(p.coefficients != NULL);
    delete_polynom(p);
    printf(" > random_polynom test passed\n");

    // Test copy_polynom
    p1 = monom(d);
    p2 = (Polynomial_t){0};
    copy_polynom(p1, &p2);
    assert(p1.degree == p2.degree);
    assert(p1.coefficients != p2.coefficients);
    for (pol_degree_t i = 0; i <= p1.degree; i++) {
        assert(p1.coefficients[i] == p2.coefficients[i]);
    }
    delete_polynom(p1);
    delete_polynom(p2);
    printf(" > copy_polynom test passed\n");

    // Test add_polynoms
    p1 = monom(d);
    p2 = monom(d);
    p3 = (Polynomial_t){0};
    add_polynoms(p1, p2, &p3);
    assert(p3.degree == 0);
    assert(p3.coefficients != NULL);
    for (pol_degree_t i = 0; i <= p3.degree; i++) {
        assert(p3.coefficients[i] == 0);
    }
    delete_polynom(p1);
    delete_polynom(p2);
    delete_polynom(p3);
    printf(" > add_polynoms test passed\n");

    // Test multiply_polynoms
    p1 = monom(d+1);
    p2 = monom(d);
    p3 = (Polynomial_t){0};
    multiply_polynoms(p1, p2, &p3);
    assert(p3.degree == 2*d+1);
    assert(p3.coefficients != NULL);
    for (pol_degree_t i = 0; i < 2*d+1; i++) {
        assert(p3.coefficients[i] == 0);
    }
    assert(p3.coefficients[2*d+1] == 1);
    delete_polynom(p1);
    delete_polynom(p2);
    delete_polynom(p3);
    p1.degree = 1;
    p1.coefficients = (bool*) malloc(2*sizeof(bool));
    p1.coefficients[0] = 1;
    p1.coefficients[1] = 1;
    p2.degree = 1;
    p2.coefficients = (bool*) malloc(2*sizeof(bool));
    p2.coefficients[0] = 1;
    p2.coefficients[1] = 1;
    multiply_polynoms(p1, p2, &p1);
    assert(p1.degree == 2);
    assert(p1.coefficients != NULL);
    assert(p1.coefficients[0] == 1);
    assert(p1.coefficients[1] == 0);
    assert(p1.coefficients[2] == 1);
    delete_polynom(p1);
    delete_polynom(p2);
    printf(" > multiply_polynoms test passed\n");

    // Test divide_polynoms
    p1 = monom(d+1);
    p2 = monom(d);
    p3 = (Polynomial_t){0};
    divide_polynoms(p1, p2, &p3);
    assert(p3.degree == 1);
    assert(p3.coefficients != NULL);
    assert(p3.coefficients[0] == 0);
    assert(p3.coefficients[1] == 1);
    delete_polynom(p1);
    delete_polynom(p2);
    delete_polynom(p3);
    p1.degree = 2;
    p1.coefficients = (bool*) malloc(3*sizeof(bool));
    p1.coefficients[0] = 1;
    p1.coefficients[1] = 0;
    p1.coefficients[2] = 1;
    p2.degree = 1;
    p2.coefficients = (bool*) malloc(2*sizeof(bool));
    p2.coefficients[0] = 1;
    p2.coefficients[1] = 1;
    divide_polynoms(p1, p2, &p1);
    assert(p1.degree == 1);
    assert(p1.coefficients != NULL);
    assert(p1.coefficients[0] == 1);
    assert(p1.coefficients[1] == 1);
    delete_polynom(p1);
    delete_polynom(p2);
    printf(" > divide_polynoms test passed\n");

    printf("Polynomial test passed\n");


    /* --- Test Homomorph ---*/
    printf("Homomorph test\n");
    bool x, y;
    Polynomial_t c = {0};
    HomomContext ctx = {0};
    
    for (uint16_t i = 1; i < nb_test+1; i++) {
        homomorph_init(d, dp, delta, tau, &ctx);

        x = (bool)rand()%2;

        Part part = random_part(tau);

        encrypt_bit(x, ctx.pk, part, &c);

        decrypt_bit(c, ctx.sk, &y);

        assert(x == y);
        printf(" > Test %d/%d passed\n", i, nb_test);

        delete_part(part);
        delete_polynom(c);
        homomorph_clear(ctx);
    }

    printf("Homomorph test passed\n");

    return 0;
}