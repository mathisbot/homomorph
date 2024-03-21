#include "homomorph.h"


static SecKey gen_secret_key(pol_degree_t d) {
    SecKey sk = random_polynom(d);
    sk.coefficients[d] = true;
    return sk;
}

static PubKey gen_public_key(SecKey sk, pol_degree_t dp, pol_degree_t delta, uint64_t tau) {
    PubKey pk;
    pk.size = tau;
    pk.elements = (Polynomial_t*) malloc(tau*sizeof(Polynomial_t));
    if (pk.elements == NULL) exit(1);
    Polynomial_t p1, p2;
    Polynomial_t tmp_pol1, tmp_pol2;
    for (uint64_t i = 0; i < tau; i++) {
        // First term
        tmp_pol1 = random_polynom(dp);
        multiply_polynoms((Polynomial_t)sk, tmp_pol1, &p1);
        delete_polynom(tmp_pol1);
        // Second term
        tmp_pol1 = monom(1);
        tmp_pol2 = random_polynom(delta);
        multiply_polynoms(tmp_pol1, tmp_pol2, &p2);
        delete_polynom(tmp_pol1);
        delete_polynom(tmp_pol2);
        // Add terms
        add_polynoms(p1, p2, &(pk.elements[i]));
        delete_polynom(p1);
        delete_polynom(p2);
    }
    return pk;
}

void homomorph_init(pol_degree_t d, pol_degree_t dp, pol_degree_t delta, uint64_t tau, HomomContext* ctx) {
    if (ctx == NULL) return;
    ctx->sk = gen_secret_key(d);
    ctx->d = d;
    ctx->dp = dp;
    ctx->delta = delta;
    ctx->tau = tau;
    ctx->pk = gen_public_key(ctx->sk, dp, delta, tau);
}

void homomorph_clear(HomomContext ctx) {
    delete_polynom(ctx.sk);
    for (uint64_t i = 0; i < ctx.tau; i++) {
        delete_polynom(ctx.pk.elements[i]);
    }
    free(ctx.pk.elements);
}

void encrypt_bit(bool bit, PubKey pk, Part part, Polynomial_t* c) {
    if (part.size < pk.size) exit(1);
    Polynomial_t p = constant_polynom(bit);
    for (uint64_t i = 0; i < pk.size; i++) {
        if (part.elements[i]) {
            add_polynoms(p, pk.elements[i], &p);
        }
    }
    *c = p;
}

void decrypt_bit(Polynomial_t c, SecKey sk, bool* bit) {
    Polynomial_t p;
    divide_polynoms(c, (Polynomial_t)sk, &p);
    *bit = p.coefficients[0];
    delete_polynom(p);
}

void encrypt(uint64_t n, PubKey pk, CipheredInt* c) {
    uint32_t num_bits = sizeof(n)*8;
    c->size = num_bits;
    c->elements = (Polynomial_t*) malloc(num_bits*sizeof(Polynomial_t));
    if (c->elements == NULL) exit(1);
    Part part;
    for (uint32_t i = 0; i < num_bits; i++) {
        part = random_part(pk.size);
        encrypt_bit((n >> i) & 1, pk, part, &(c->elements[i]));
        delete_part(part);
    }
}

void decrypt(CipheredInt* c, SecKey sk, uint64_t* n) {
    if (c->size >= sizeof(uint64_t)*8) exit(1);
    *n = 0;
    bool bit;
    for (uint32_t i = 0; i < c->size; i++) {
        decrypt_bit(c->elements[i], sk, &bit);
        *n |= (bit << i);
    }
}
