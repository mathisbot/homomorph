#include "polynom.h"


Polynomial_t monom(pol_degree_t degree) {
    Polynomial_t p = {0};
    if (degree > MAX_POLYNOM_DEGREE) exit(EXIT_BAD_DEGREE);
    p.degree = degree;
    p.coefficients = (bool*) malloc((degree+1) * sizeof(bool));
    if (p.coefficients == NULL) exit(1);
    for (pol_degree_t i = 0; i < degree; i++) {
        p.coefficients[i] = false;
    }
    p.coefficients[degree] = true;
    return p;
}

Polynomial_t constant_polynom(bool value) {
    Polynomial_t p = {0};
    // p.degree = 0;
    p.coefficients = (bool*) malloc(sizeof(bool));
    if (p.coefficients == NULL) exit(1);
    p.coefficients[0] = value;
    return p;
}

Polynomial_t random_polynom(pol_degree_t degree) {
    Polynomial_t p = {0};
    if (degree > MAX_POLYNOM_DEGREE) exit(EXIT_BAD_DEGREE);
    p.degree = degree;
    p.coefficients = (bool*) malloc((degree+1) * sizeof(bool));
    if (p.coefficients == NULL) exit(1);
    for (pol_degree_t i = 0; i <= degree; i++) {
        // Assume that the random function has been seeded
        p.coefficients[i] = rand() % 2;
    }
    return p;
}

void copy_polynom(Polynomial_t src, Polynomial_t* dest) {
    dest->degree = src.degree;
    dest->coefficients = (bool*) malloc((src.degree+1) * sizeof(bool));
    for (pol_degree_t i = 0; i <= src.degree; i++) {
        dest->coefficients[i] = src.coefficients[i];
    }
}

void delete_polynom(Polynomial_t p) {
    free(p.coefficients);
    p.coefficients = NULL;
}

void add_polynoms(Polynomial_t p1, Polynomial_t p2, Polynomial_t* p) {
    if (p == NULL) exit(1);
    p->degree = MAX(p1.degree, p2.degree);
    p->coefficients = (bool*) malloc((p1.degree+1) * sizeof(bool));
    if (p->coefficients == NULL) exit(1);

    #if !SSE2_SUPPORTED
    for (pol_degree_t i = 0; i <= p->degree; i++) {
        if (i > p1.degree) p->coefficients[i] = p2.coefficients[i];
        else if (i > p2.degree) p->coefficients[i] = p1.coefficients[i];
        else p->coefficients[i] = p1.coefficients[i] ^ p2.coefficients[i];
    }

    #else
    __m128i* coeffs1 = (__m128i*)p1.coefficients;
    __m128i* coeffs2 = (__m128i*)p2.coefficients;
    __m128i* result_coeffs_mm = (__m128i*)result_coeffs;
    
    for (pol_degree_t i = 0; i <= p->degree / 128; ++i) {
        result_coeffs_mm[i] = _mm_xor_si128(coeffs1[i], coeffs2[i]);
    }

    p->coefficients = result_coeffs;
    p->degree = result_degree;

    #endif
}

void substract_polynoms(Polynomial_t p1, Polynomial_t p2, Polynomial_t* p) {
    if (p == NULL) exit(1);
    p->degree = MAX(p1.degree, p2.degree);
    p->coefficients = (bool*) malloc((p1.degree+1) * sizeof(bool));
    if (p->coefficients == NULL) exit(1);

    #if !SSE2_SUPPORTED
    for (pol_degree_t i = 0; i <= p->degree; i++) {
        if (i > p1.degree) p->coefficients[i] = p2.coefficients[i];
        else if (i > p2.degree) p->coefficients[i] = p1.coefficients[i];
        else p->coefficients[i] = p1.coefficients[i] ^ p2.coefficients[i];
    }

    #else
    __m128i* coeffs1 = (__m128i*)p1.coefficients;
    __m128i* coeffs2 = (__m128i*)p2.coefficients;
    __m128i* result_coeffs_mm = (__m128i*)result_coeffs;
    
    for (pol_degree_t i = 0; i <= p->degree / 128; ++i) {
        result_coeffs_mm[i] = _mm_xor_si128(coeffs1[i], coeffs2[i]);
    }

    p->coefficients = result_coeffs;
    p->degree = result_degree;

    #endif
}

void multiply_polynoms(Polynomial_t p1, Polynomial_t p2, Polynomial_t* p) {
    if (p == NULL) exit(1);
    p->degree = p1.degree + p2.degree;
    p->coefficients = (bool*) malloc((p->degree+1) * sizeof(bool));
    if (p->coefficients == NULL) exit(1);
    for (pol_degree_t i = 0; i <= p->degree; i++) p->coefficients[i] = false;
    
    #if !SSE2_SUPPORTED
    for (pol_degree_t i = 0; i <= p1.degree; i++) {
        for (pol_degree_t j = 0; j <= p2.degree; j++) {
            p->coefficients[i+j] ^= p1.coefficients[i] & p2.coefficients[j];
        }
    }

    #else
    __m128i* coeffs1 = (__m128i*)p1.coefficients;
    __m128i* coeffs2 = (__m128i*)p2.coefficients;
    
    for (pol_degree_t i = 0; i <= p1.degree / 128; ++i) {
        for (pol_degree_t j = 0; j <= p2.degree / 128; ++j) {
            __m128i coeff_product = _mm_and_si128(coeffs1[i], coeffs2[j]);
            for (int k = 0; k < 128; ++k) {
                p->coefficients[i * 128 + j * 128 + k] ^= (coeff_product.m128i_i8[k / 8] >> (k % 8)) & 1;
            }
        }
    }
    #endif
}

void divide_polynoms(Polynomial_t p1, Polynomial_t p2, Polynomial_t* p) {
    if (p2.degree == 0 && p2.coefficients[0] == false) {
        exit(1);
    }
    p->degree = p1.degree - p2.degree;
    p->coefficients = (bool*)malloc((p->degree + 1) * sizeof(bool));
    if (p->coefficients == NULL) {
        exit(EXIT_FAILURE);
    }
    memset(p->coefficients, 0, (p->degree + 1) * sizeof(bool));

    #if !SSE2_SUPPORTED
    Polynomial_t remainder = p1;
    while (remainder.degree >= p2.degree) {
        pol_degree_t shift = remainder.degree - p2.degree;
        p->coefficients[shift] = true;
        for (pol_degree_t i = 0; i <= p2.degree; ++i) {
            if (p2.coefficients[i]) {
                remainder.coefficients[i + shift] ^= true;
            }
        }
        while (remainder.degree > 0 && !remainder.coefficients[remainder.degree])
            --remainder.degree;
    }
    delete_polynom(remainder);
    #else
    __m128i* coeffs1 = (__m128i*)p1.coefficients;
    __m128i* coeffs2 = (__m128i*)p2.coefficients;
    __m128i* result = (__m128i*)p->coefficients;

    for (pol_degree_t i = 0; i <= p1.degree / 128; ++i) {
        for (pol_degree_t j = 0; j <= p2.degree / 128; ++j) {
            __m128i coeff_division = _mm_xor_si128(coeffs1[i], coeffs2[j]);
            for (int k = 0; k < 128; ++k) {
                result[i * 128 + j * 128 + k] = (coeff_division.m128i_i8[k / 8] >> (k % 8)) & 1;
            }
        }
    }
    #endif
}

bool evaluate_polynom(Polynomial_t p, bool x) {
    bool result = p.coefficients[0];
    for (pol_degree_t i = 1; i <= p.degree; i++) {
        result ^= p.coefficients[i] & x;
        x = x & x;
    }
    return result;
}
