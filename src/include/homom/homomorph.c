#include "homomorph.h"


static Polynomial_t gen_secret_key(pol_degree_t d) {
    Polynomial_t sk = random_polynom(d);
    sk.coefficients[d] = true;
    return sk;
}

static PubKey gen_public_key(Polynomial_t sk, pol_degree_t dp, pol_degree_t delta, uint64_t tau) {
    PubKey pk;
    pk.size = tau;
    Polynomial_t p1;
    Polynomial_t p2;
    pk.elements = (Polynomial_t*) malloc(sizeof(Polynomial_t));
    for (uint64_t i = 0; i < tau; i++) {
        Polynomial_t tmp_pol1, tmp_pol2;
        // First term
        tmp_pol1 = random_polynom(dp);
        multiply_polynoms(sk, tmp_pol1, &p1);
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
    if (part.size != pk.size) exit(1);
    Polynomial_t p, bit_pol;
    bit_pol = constant_polynom(bit);
    p = constant_polynom(0);
    for (uint64_t i = 0; i < part.size; i++) {
        if (part.elements[i]) {
            add_polynoms(p, pk.elements[i], &p);
        }
    }
    add_polynoms(bit_pol, p, c);
}

void decrypt_bit(Polynomial_t c, Polynomial_t sk, bool* bit) {
    Polynomial_t p;
    divide_polynoms(c, sk, &p);
    *bit = evaluate_polynom(p, 0);
    delete_polynom(p);
}
