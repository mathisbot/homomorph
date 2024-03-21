#include "polynom.h"


static pol_degree_t degree_of_polynom(Polynomial_t p) {
    for (pol_degree_t i = p.degree; i > 0; i--) {
        if (p.coefficients[i]) return i;
    }
    return 0;
}


Polynomial_t monom(pol_degree_t degree) {
    Polynomial_t p = {0};
    if (degree > MAX_POLYNOM_DEGREE) exit(EXIT_BAD_DEGREE);
    p.degree = degree;
    p.size = degree + 1;
    p.coefficients = (bool*) malloc(p.size*sizeof(bool));
    if (p.coefficients == NULL) exit(1);
    for (pol_degree_t i = 0; i < degree; i++) {
        p.coefficients[i] = false;
    }
    p.coefficients[degree] = true;
    return p;
}

Polynomial_t constant_polynom(bool value) {
    Polynomial_t p = {0};
    p.degree = 0;
    p.size = 1;
    p.coefficients = (bool*) malloc(sizeof(bool));
    if (p.coefficients == NULL) exit(1);
    p.coefficients[0] = value;
    return p;
}

Polynomial_t random_polynom(pol_degree_t degree) {
    Polynomial_t p = {0};
    if (degree > MAX_POLYNOM_DEGREE) exit(EXIT_BAD_DEGREE);
    p.degree = degree;
    p.size = degree+1;
    p.coefficients = (bool*) malloc(p.size*sizeof(bool));
    if (p.coefficients == NULL) exit(1);
    for (pol_degree_t i = 0; i <= degree; i++) {
        // Assume that the random function has been seeded
        p.coefficients[i] = rand() % 2;
    }
    return p;
}

void copy_polynom(Polynomial_t src, Polynomial_t* dest) {
    if (dest == NULL) exit(1);
    dest->degree = src.degree;
    dest->size = src.size;
    dest->coefficients = (bool*) malloc(src.size*sizeof(bool));
    for (pol_degree_t i = 0; i <= src.degree; i++) {
        dest->coefficients[i] = src.coefficients[i];
    }
}

void delete_polynom(Polynomial_t p) {
    // Currently, if p.coefficients is uninitialized, this will cause a segfault
    free(p.coefficients);
}

void add_polynoms(Polynomial_t p1, Polynomial_t p2, Polynomial_t* p) {
    if (p == NULL) exit(1);
    p->size = MAX(p1.degree, p2.degree) + 1;
    p->coefficients = (bool*) malloc(p->size*sizeof(bool));
    if (p->coefficients == NULL) exit(1);

    for (pol_degree_t i = 0; i < p->size; i++) {
        if (i > p1.degree) p->coefficients[i] = p2.coefficients[i];
        else if (i > p2.degree) p->coefficients[i] = p1.coefficients[i];
        else p->coefficients[i] = p1.coefficients[i] ^ p2.coefficients[i];
    }

    if (p1.degree != p2.degree) p->degree = MAX(p1.degree, p2.degree);
    else {
        p->degree = degree_of_polynom(*p);
        p->size = p->degree + 1;
        p->coefficients = (bool*) realloc(p->coefficients, p->size*sizeof(bool));
    }
}

void substract_polynoms(Polynomial_t p1, Polynomial_t p2, Polynomial_t* p) {
    // In Z/2Z[X], addition and substraction are the same operation
    add_polynoms(p1, p2, p);
}

void multiply_polynoms(Polynomial_t p1, Polynomial_t p2, Polynomial_t* p) {
    if (p == NULL) exit(1);
    // This is done to avoid problems if p is pointing to one of the original polynomals
    Polynomial_t p1c, p2c;
    copy_polynom(p1, &p1c);
    copy_polynom(p2, &p2c);
    p->degree = p1c.degree + p2c.degree;
    p->size = p->degree + 1;
    // if (&p1 == p || &p2 == p) delete_polynom(*p);  // Make sure we don't have any memory leaks
    p->coefficients = (bool*) malloc(p->size * sizeof(bool));
    if (p->coefficients == NULL) exit(1);
    for (pol_degree_t i = 0; i < p->size; i++) p->coefficients[i] = false;
    
    for (pol_degree_t i = 0; i <= p1c.degree; i++) {
        for (pol_degree_t j = 0; j <= p2c.degree; j++) {
            p->coefficients[i+j] ^= p1c.coefficients[i] & p2c.coefficients[j];
        }
    }

    delete_polynom(p1c);
    delete_polynom(p2c);
}


void divide_polynoms(Polynomial_t p1, Polynomial_t p2, Polynomial_t* p) {
    if (p == NULL) exit(1);
    if (p2.degree == 0 && p2.coefficients[0] == false) exit(EXIT_DIVISION_BY_ZERO);
    // This is done to avoid problems with the original polynoms if p is pointing to one of them
    Polynomial_t p1c, p2c;
    copy_polynom(p1, &p1c);
    copy_polynom(p2, &p2c);
    p->degree = p1c.degree - p2c.degree;
    p->size = p->degree + 1;
    // if (&p1 == p || &p2 == p) delete_polynom(*p);  // Make sure we don't have any memory leaks
    p->coefficients = (bool*)malloc(p->size * sizeof(bool));
    if (p->coefficients == NULL) exit(EXIT_FAILURE);
    for (pol_degree_t i = 0; i <= p->degree; i++) p->coefficients[i] = false;

    Polynomial_t remainder = {0};
    copy_polynom(p1c, &remainder);
    while (remainder.degree >= p2c.degree) {
        Polynomial_t monom_division = monom(remainder.degree - p2c.degree);
        Polynomial_t monom_product = {0};
        multiply_polynoms(monom_division, p2c, &monom_product);
        substract_polynoms(remainder, monom_product, &remainder);
        add_polynoms(*p, monom_division, p);
        delete_polynom(monom_division);
        delete_polynom(monom_product);
    }

    delete_polynom(remainder);
    delete_polynom(p1c);
    delete_polynom(p2c);
}
