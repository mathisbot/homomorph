#pragma once

#include "polynom.h"

typedef struct {
    Polynomial_t* elements;
    uint64_t size;
} PubKey;

/**
 * @brief Generates a public and a secret key for the homomorphic encryption scheme
 * 
 * Memory is allocated for the public and secret keys.
 * 
 * @param[in] d The degree of the secret key
 * @param[in] dp The degree of the random element
 * @param[in] delta The degree of the random element
 * @param[in] tau The size of the public key
 * @param[out] pk The public key
 * @param[out] sk The secret key
 */
void homomorph_keygen(pol_degree_t d, pol_degree_t dp, pol_degree_t delta, uint64_t tau, PubKey* pk, Polynomial_t* sk);

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
void encrypt_bit(bool bit, PubKey* pk, Part* part, Polynomial_t* c);

/**
 * @brief Decrypts a bit using the secret key
 * 
 * @param[in] c The encrypted bit
 * @param[in] sk The secret key
 * @param[out] bit The decrypted bit
 */
void decrypt_bit(Polynomial_t* c, Polynomial_t* sk, bool* bit);