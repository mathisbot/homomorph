#pragma once

#include "polynom.h"

typedef struct {
    Polynomial_t* elements;
    uint64_t size;
} PubKey;

typedef Polynomial_t SecKey;

typedef struct {
    PubKey pk;
    SecKey sk;
    pol_degree_t d;
    pol_degree_t dp;
    pol_degree_t delta;
    uint64_t tau;
} HomomContext;

typedef struct {
    Polynomial_t elements[sizeof(uint64_t)];
} CipheredInt;

/**
 * @brief Generates a public and a secret key for the homomorphic encryption scheme
 * 
 * @param[in] d The degree of the secret key
 * @param[in] dp The degree of the random element
 * @param[in] delta The degree of the random element
 * @param[in] tau The size of the public key
 * @param[out] ctx Pointer to the context to store the public and secret keys
*/
void homomorph_init(pol_degree_t d, pol_degree_t dp, pol_degree_t delta, uint64_t tau, HomomContext* ctx);

/**
 * @brief Deletes the context
 * 
 * @param[in] ctx The context to delete
*/
void homomorph_clear(HomomContext ctx);

/**
 * @brief Encrypts a bit using the public key and a part
 * 
 * @param[in] bit The bit to be encrypted
 * @param[in] pk The public key
 * @param[in] part The part to be encrypted
 * @param[out] c The encrypted bit
 * 
 * @see Part
*/
void encrypt_bit(bool bit, PubKey pk, Part part, Polynomial_t* c);

/**
 * @brief Decrypts a bit using the secret key
 * 
 * @param[in] c The encrypted bit
 * @param[in] sk The secret key
 * @param[out] bit The decrypted bit
*/
void decrypt_bit(Polynomial_t c, SecKey sk, bool* bit);

/**
 * @brief Encrypts an integer using the public key
 * 
 * @param[in] bit The bit to be encrypted
 * @param[in] pk The public key
 * @param[out] c The encrypted integer
*/
void encrypt(uint64_t n, PubKey pk, CipheredInt* c);

/**
 * @brief Decrypts an integer using the secret key
 * 
 * @param[in] c A pointer to the encrypted integer
 * @param[in] sk The secret key
 * @param[out] bit The decrypted integer
*/
void decrypt(CipheredInt* c, SecKey sk, uint64_t* n);

/**
 * @brief Adds two encrypted integers
 * 
 * @param[in] a The first encrypted integer
 * @param[in] b The second encrypted integer
 * @param[in] pk The public key
 * @param[out] c The result of the addition
*/
void ciphered_add(CipheredInt a, CipheredInt b, CipheredInt* c);