#include "homomorph.h"


#include <stdio.h>
static Polynomial_t* gen_secret_key(pol_degree_t d) {
    Polynomial_t* sk;
    random_polynom(d, sk);
    if (sk == NULL) return NULL;
    sk->coefficients[0] = true;
    return sk;
}

static PubKey* gen_public_key(Polynomial_t* sk, pol_degree_t dp, pol_degree_t delta, uint64_t tau) {
    PubKey* pk = (PubKey*) malloc(sizeof(PubKey));
    if (pk == NULL) return NULL;
    Polynomial_t* p1 = NULL;
    Polynomial_t* p2 = NULL;
    for (uint64_t i = 0; i < tau; i++) {
        Polynomial_t *tmp_pol1, *tmp_pol2;
        // First term
        random_polynom(dp, tmp_pol1);
        multiply_polynoms(sk, tmp_pol1, p1);
        delete_polynom(tmp_pol1);
        // Second term
        monom(1, tmp_pol1);
        random_polynom(dp, tmp_pol2);
        multiply_polynoms(tmp_pol1, tmp_pol2, p2);
        delete_polynom(tmp_pol1);
        delete_polynom(tmp_pol2);
        // Add terms
        add_polynoms(p1, p2, &(pk->elements[i]));
        delete_polynom(p1);
        delete_polynom(p2);
    }
    return pk;
}

void homomorph_keygen(pol_degree_t d, pol_degree_t dp, pol_degree_t delta, uint64_t tau, PubKey* pk, Polynomial_t* sk) {
    sk = gen_secret_key(d);
    printf("Secret key generated.\n");
    pk = gen_public_key(sk, dp, delta, tau);
    return;
}

void encrypt_bit(bool bit, PubKey* pk, Part* part, Polynomial_t* c) {
    if (pk == NULL || part == NULL) return;
    if (part->size != pk->size) return;
    Polynomial_t* p, *bit_pol;
    constant_polynom(bit, bit_pol);
    constant_polynom(0, bit_pol);
    for (uint64_t i = 0; i < part->size; i++) {
        if (part->elements[i]) {
            add_polynoms(p, &(pk->elements[i]), p);
        }
    }
    copy_polynom(p, c);
}

void decrypt_bit(Polynomial_t* c, Polynomial_t* sk, bool* bit) {
    if (c == NULL || sk == NULL) return;
    Polynomial_t* p;
    divide_polynoms(c, sk, p);
    *bit = evaluate_polynom(p, 0);
    delete_polynom(p);
}
