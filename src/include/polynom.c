#include "polynom.h"


Polynom* monom(pol_degree_t degree) {
    if (degree < 0 || degree > MAX_POLYNOM_DEGREE) return NULL;
    Polynom* p = (Polynom*) malloc(sizeof(Polynom));
    if (p == NULL) return NULL;
    p->degree = degree;
    p->coefficients = (bool*) malloc((degree+1) * sizeof(bool));
    for (pol_degree_t i = 0; i <= degree; i++) {
        p->coefficients[i] = false;
    }
    p->coefficients[degree] = true;
    return p;
}

Polynom* constant_polynom(bool value) {
    Polynom* p = (Polynom*) malloc(sizeof(Polynom));
    if (p == NULL) return NULL;
    p->degree = 0;
    p->coefficients = (bool*) malloc(sizeof(bool));
    p->coefficients[0] = value;
    return p;
}

Polynom* random_polynom(pol_degree_t degree) {
    if (degree < 0 || degree > MAX_POLYNOM_DEGREE) return NULL;
    Polynom* p = (Polynom*) malloc(sizeof(Polynom));
    if (p == NULL) return NULL;
    p->degree = degree;
    p->coefficients = (bool*) malloc((degree+1) * sizeof(bool));
    for (pol_degree_t i = 0; i <= degree; i++) {
        p->coefficients[i] = rand() % 2;
    }
    return p;
}

Polynom* copy_polynom(Polynom* p) {
    if (p == NULL) return NULL;
    Polynom* copy = (Polynom*) malloc(sizeof(Polynom));
    if (copy == NULL) return NULL;
    copy->degree = p->degree;
    copy->coefficients = (bool*) malloc((p->degree+1) * sizeof(bool));
    for (pol_degree_t i = 0; i <= p->degree; i++) {
        copy->coefficients[i] = p->coefficients[i];
    }
    return copy;
}

void delete_polynom(Polynom* p) {
    if (p == NULL) return;
    free(p->coefficients);
    free(p);
}

Polynom* add_polynoms(Polynom* p1, Polynom* p2) {
    if (p1 == NULL || p2 == NULL || p1->degree != p2->degree) return NULL;
    Polynom* sum = (Polynom*) malloc(sizeof(Polynom));
    if (sum == NULL) return NULL;
    sum->degree = MAX(p1->degree, p2->degree);
    sum->coefficients = (bool*) malloc((p1->degree+1) * sizeof(bool));
    if (sum->coefficients == NULL) { free(sum); return NULL; }

    #if !SSE2_SUPPORTED
    for (pol_degree_t i = 0; i <= sum->degree; i++) {
        if (i > p1->degree) sum->coefficients[i] = p2->coefficients[i];
        else if (i > p2->degree) sum->coefficients[i] = p1->coefficients[i];
        else sum->coefficients[i] = p1->coefficients[i] ^ p2->coefficients[i];
    }

    #else
    __m128i* coeffs1 = (__m128i*)p1->coefficients;
    __m128i* coeffs2 = (__m128i*)p2->coefficients;
    __m128i* result_coeffs_mm = (__m128i*)result_coeffs;
    
    for (pol_degree_t i = 0; i <= sum->degree / 128; ++i) {
        result_coeffs_mm[i] = _mm_xor_si128(coeffs1[i], coeffs2[i]);
    }

    result->coefficients = result_coeffs;
    result->degree = result_degree;

    #endif
    return sum;
}

Polynom* multiply_polynoms(Polynom* p1, Polynom* p2) {
    if (p1 == NULL || p2 == NULL) return NULL;
    Polynom* product = (Polynom*) malloc(sizeof(Polynom));
    if (product == NULL) return NULL;
    product->degree = p1->degree + p2->degree;
    product->coefficients = (bool*) malloc((product->degree+1) * sizeof(bool));
    if (product->coefficients == NULL) { free(product); return NULL; }
    
    #if !SSE2_SUPPORTED
    for (pol_degree_t i = 0; i <= p1->degree; i++) {
        for (pol_degree_t j = 0; j <= p2->degree; j++) {
            product->coefficients[i+j] ^= p1->coefficients[i] & p2->coefficients[j];
        }
    }

    #else
    __m128i* coeffs1 = (__m128i*)p1->coefficients;
    __m128i* coeffs2 = (__m128i*)p2->coefficients;
    
    for (pol_degree_t i = 0; i <= p1->degree / 128; ++i) {
        for (pol_degree_t j = 0; j <= p2->degree / 128; ++j) {
            __m128i coeff_product = _mm_and_si128(coeffs1[i], coeffs2[j]);
            for (int k = 0; k < 128; ++k) {
                product->coefficients[i * 128 + j * 128 + k] ^= (coeff_product.m128i_i8[k / 8] >> (k % 8)) & 1;
            }
        }
    }
    #endif
    
    return product;
}


Polynom* divide_polynoms(Polynom* p1, Polynom* p2) {
    if (p1 == NULL || p2 == NULL || p2->degree > p1->degree) return NULL;
    Polynom* quotient = (Polynom*) malloc(sizeof(Polynom));
    if (quotient == NULL) return NULL;
    quotient->degree = p1->degree - p2->degree;
    quotient->coefficients = (bool*) malloc((quotient->degree+1) * sizeof(bool));
    if (quotient->coefficients == NULL) {  free(quotient); return NULL; }
    
    Polynom* remainder = copy_polynom(p1);
    Polynom* temp = monom(0);
    
    #if !AVX2_SUPPORTED
    while (remainder->degree >= p2->degree) {
        temp->degree = remainder->degree - p2->degree;
        temp->coefficients = (bool*) realloc(temp->coefficients, (temp->degree+1) * sizeof(bool));
        for (pol_degree_t i = 0; i <= temp->degree; i++) temp->coefficients[i] = false;
        temp->coefficients[temp->degree] = true;
        Polynom* product = multiply_polynoms(p2, temp);
        Polynom* sum = add_polynoms(quotient, temp);
        delete_polynom(quotient);
        quotient = sum;
        sum = add_polynoms(remainder, product);
        delete_polynom(remainder);
        remainder = sum;
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
        quotient = add_polynoms(quotient, temp);
        remainder->coefficients = (bool*)realloc(remainder->coefficients, (temp->degree + 1) * sizeof(bool));
        memcpy(remainder->coefficients, sum_coeffs, (temp->degree + 1) * sizeof(bool));
        remainder->degree = temp->degree;
        free(product_coeffs);
        free(sum_coeffs);
    }
    #endif
    
    delete_polynom(remainder);
    delete_polynom(temp);
    
    return quotient;
}

bool evaluate_polynom(Polynom* p, bool x) {
    if (p == NULL) return false;
    bool result = p->coefficients[0];
    for (pol_degree_t i = 1; i <= p->degree; i++) {
        result ^= p->coefficients[i] & x;
        x = x & x;
    }
    return result;
}

void print_polynom(Polynom* p) {
    if (p == NULL) return;
    printf("Polynom of degree %d: ", p->degree);
    for (pol_degree_t i = 0; i <= p->degree; i++) {
        if (p->coefficients[i]) {
            printf("x^%d + ", i);
        }
    }
    printf("\b\b \n");
}
