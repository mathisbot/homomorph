#include "polynom.h"


void monom(pol_degree_t degree, Polynomial_t* p) {
    if (degree < 0 || degree > MAX_POLYNOM_DEGREE) p=NULL;
    p = (Polynomial_t*) malloc(sizeof(Polynomial_t));
    if (p == NULL) p=NULL;
    p->degree = degree;
    p->coefficients = (bool*) malloc((degree+1) * sizeof(bool));
    for (pol_degree_t i = 0; i <= degree; i++) {
        p->coefficients[i] = false;
    }
    p->coefficients[degree] = true;
}

void constant_polynom(bool value, Polynomial_t* p) {
    p = (Polynomial_t*) malloc(sizeof(Polynomial_t));
    if (p == NULL) p=NULL;
    p->degree = 0;
    p->coefficients = (bool*) malloc(sizeof(bool));
    p->coefficients[0] = value;
}

void random_polynom(pol_degree_t degree, Polynomial_t* p) {
    if (degree < 0 || degree > MAX_POLYNOM_DEGREE) p=NULL;
    p = (Polynomial_t*) malloc(sizeof(Polynomial_t));
    if (p == NULL) p=NULL;
    p->degree = degree;
    p->coefficients = (bool*) malloc((degree+1) * sizeof(bool));
    if (p->coefficients == NULL) { free(p); p=NULL; }
    for (pol_degree_t i = 0; i <= degree; i++) {
        p->coefficients[i] = rand() % 2;
    }
}

void copy_polynom(Polynomial_t* src, Polynomial_t* dest) {
    if (src == NULL) dest=NULL;
    dest = (Polynomial_t*) malloc(sizeof(Polynomial_t));
    if (dest == NULL) return;
    dest->degree = src->degree;
    dest->coefficients = (bool*) malloc((src->degree+1) * sizeof(bool));
    for (pol_degree_t i = 0; i <= src->degree; i++) {
        dest->coefficients[i] = src->coefficients[i];
    }
}

void delete_polynom(Polynomial_t* p) {
    if (p == NULL) return;
    free(p->coefficients);
    free(p);
}

void add_polynoms(Polynomial_t* p1, Polynomial_t* p2, Polynomial_t* p) {
    if (p1 == NULL || p2 == NULL || p1->degree != p2->degree) p=NULL;
    p = (Polynomial_t*) malloc(sizeof(Polynomial_t));
    if (p == NULL) p=NULL;
    p->degree = MAX(p1->degree, p2->degree);
    p->coefficients = (bool*) malloc((p1->degree+1) * sizeof(bool));
    if (p->coefficients == NULL) { free(p); p=NULL; }

    #if !SSE2_SUPPORTED
    for (pol_degree_t i = 0; i <= p->degree; i++) {
        if (i > p1->degree) p->coefficients[i] = p2->coefficients[i];
        else if (i > p2->degree) p->coefficients[i] = p1->coefficients[i];
        else p->coefficients[i] = p1->coefficients[i] ^ p2->coefficients[i];
    }

    #else
    __m128i* coeffs1 = (__m128i*)p1->coefficients;
    __m128i* coeffs2 = (__m128i*)p2->coefficients;
    __m128i* result_coeffs_mm = (__m128i*)result_coeffs;
    
    for (pol_degree_t i = 0; i <= p->degree / 128; ++i) {
        result_coeffs_mm[i] = _mm_xor_si128(coeffs1[i], coeffs2[i]);
    }

    p->coefficients = result_coeffs;
    p->degree = result_degree;

    #endif
}

void multiply_polynoms(Polynomial_t* p1, Polynomial_t* p2, Polynomial_t* p) {
    if (p1 == NULL || p2 == NULL) p=NULL;
    p = (Polynomial_t*) malloc(sizeof(Polynomial_t));
    if (p == NULL) p=NULL;
    p->degree = p1->degree + p2->degree;
    p->coefficients = (bool*) malloc((p->degree+1) * sizeof(bool));
    if (p->coefficients == NULL) { free(p); p=NULL; }
    
    #if !SSE2_SUPPORTED
    for (pol_degree_t i = 0; i <= p1->degree; i++) {
        for (pol_degree_t j = 0; j <= p2->degree; j++) {
            p->coefficients[i+j] ^= p1->coefficients[i] & p2->coefficients[j];
        }
    }

    #else
    __m128i* coeffs1 = (__m128i*)p1->coefficients;
    __m128i* coeffs2 = (__m128i*)p2->coefficients;
    
    for (pol_degree_t i = 0; i <= p1->degree / 128; ++i) {
        for (pol_degree_t j = 0; j <= p2->degree / 128; ++j) {
            __m128i coeff_product = _mm_and_si128(coeffs1[i], coeffs2[j]);
            for (int k = 0; k < 128; ++k) {
                p->coefficients[i * 128 + j * 128 + k] ^= (coeff_product.m128i_i8[k / 8] >> (k % 8)) & 1;
            }
        }
    }
    #endif
}


void divide_polynoms(Polynomial_t* p1, Polynomial_t* p2, Polynomial_t* p) {
    if (p1 == NULL || p2 == NULL || p2->degree > p1->degree) p=NULL;
    p = (Polynomial_t*) malloc(sizeof(Polynomial_t));
    if (p == NULL) p=NULL;
    p->degree = p1->degree - p2->degree;
    p->coefficients = (bool*) malloc((p->degree+1) * sizeof(bool));
    if (p->coefficients == NULL) {  free(p); p=NULL; }
    
    Polynomial_t *remainder, *temp;
    copy_polynom(p1, remainder);
    monom(0, temp);
    
    #if !AVX2_SUPPORTED
    while (remainder->degree >= p2->degree) {
        temp->degree = remainder->degree - p2->degree;
        temp->coefficients = (bool*) realloc(temp->coefficients, (temp->degree+1) * sizeof(bool));
        for (pol_degree_t i = 0; i <= temp->degree; i++) temp->coefficients[i] = false;
        temp->coefficients[temp->degree] = true;
        Polynomial_t *product, *sum;
        multiply_polynoms(p2, temp, product);
        add_polynoms(p, temp, sum);
        delete_polynom(p);
        p = sum;
        // add_polynoms(remainder, product, sum);
        delete_polynom(remainder);
        // remainder = sum;
        delete_polynom(product);
        delete_polynom(temp);
    }
    #else
    __m128i* coeffs1 = (__m128i*)remainder->coefficients;
    __m128i* coeffs2 = (__m128i*)p2->coefficients;
    
    while (remainder->degree >= p2->degree) {
        temp->degree = remainder->degree - p2->degree;
        temp->coefficients = (bool*) realloc(temp->coefficients, (temp->degree+1) * sizeof(bool));
        for (pol_degree_t i = 0; i <= temp->degree; i++) temp->coefficients[i] = false;
        temp->coefficients[temp->degree] = true;
        __m128i* temp_coeffs = (__m128i*)temp->coefficients;
        __m128i* product_coeffs = (__m128i*)malloc((temp->degree + 1) * sizeof(__m128i));
        for (pol_degree_t i = 0; i <= temp->degree / 128; ++i) product_coeffs[i] = _mm_and_si128(coeffs2[i], temp_coeffs[0]);
        __m128i* sum_coeffs = (__m128i*)malloc((temp->degree + 1) * sizeof(__m128i));
        for (pol_degree_t i = 0; i <= temp->degree / 128; ++i) sum_coeffs[i] = _mm_xor_si128(coeffs1[i], product_coeffs[i]);
        p = add_polynoms(p, temp);
        // remainder->coefficients = (bool*)realloc(remainder->coefficients, (temp->degree + 1) * sizeof(bool));
        // memcpy(remainder->coefficients, sum_coeffs, (temp->degree + 1) * sizeof(bool));
        // remainder->degree = temp->degree;
        free(product_coeffs);
        free(sum_coeffs);
    }
    #endif
    
    delete_polynom(remainder);
    delete_polynom(temp);
}

bool evaluate_polynom(Polynomial_t* p, bool x) {
    if (p == NULL) return false;
    bool result = p->coefficients[0];
    for (pol_degree_t i = 1; i <= p->degree; i++) {
        result ^= p->coefficients[i] & x;
        x = x & x;
    }
    return result;
}
