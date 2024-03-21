#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <emmintrin.h>

#include "utils.h"

// // SSE2 support
// #ifdef __SSE2__
// #define SSE2_SUPPORTED 1
// #define SSE2_VECTOR_SIZE 16 // 128 bits = 16 bytes
// #else
// #define SSE2_SUPPORTED 0
// #endif
#define SSE2_SUPPORTED 0  // TODO: remove

#define pol_degree_t uint32_t
#define MAX_POLYNOM_DEGREE 4294967295

#define EXIT_BAD_DEGREE 2
#define EXIT_BAD_COEFFICIENT 3
#define EXIT_DIVISION_BY_ZERO 4


/**
 * @file polynom.h
 * @brief Polynomial_t structure and functions to manipulate polynoms.
 * 
 * This file contains the definition of the Polynomial_t structure and functions to manipulate polynoms.
 * Polynoms are in Z/2Z[X], the ring of polynoms with coefficients in Z/2Z.
 * 
 * @see Polynomial_t
*/


/**
 * @brief Polynomial_t structure
 * 
 * This structure is used to represent a polynom. It contains a pointer to an array of booleans, which represents the coefficients of the polynom in Z/2Z, and the degree of the polynom.
 * Degree is such that coefficients[degree] = 1 and coefficients[i] = 0 for i > degree.
 * 
 * @param coefficients Pointer to an array of booleans, which represents the coefficients of the polynomial in Z/2Z.
 * @param degree Degree of the polynomial.
 * @param size Size of the coefficients array.
 * 
 * @see pol_degree_t
*/
typedef struct {
    bool* coefficients;
    pol_degree_t degree;
    pol_degree_t size;
} Polynomial_t;


/**
 * @brief Create a new monomial
 * 
 * This function creates a new monomial of the given degree. The coefficients of the polynom are initialized to false, expect for one of degree degree.
 * 
 * @param[in] degree Degree of the polynom.
 * @return Monomial of the given degree.
 * 
 * @see Polynomial_t
*/
Polynomial_t monom(pol_degree_t degree);

/**
 * @brief Create a constant polynom
 * 
 * This function returns a constant polynom with the given value.
 * 
 * @param[in] value Value of the constant polynom
 * @return Constant polynomial
 *  
 * @see Polynomial_t
*/
Polynomial_t constant_polynom(bool value);

/**
 * @brief Create a random polynom
 * 
 * This function creates a new polynom with the given degree. The coefficients of the polynom are initialized to random values.
 * 
 * @param[in] degree Degree of the polynom.
 * @return Polynomial with random coefficients.
 * 
 * @note This function assumes that the random function has been seeded.
 * 
 * @see Polynomial_t
 * @see rand
*/
Polynomial_t random_polynom(pol_degree_t degree);

/**
 * @brief Create a copy of a polynom
 * 
 * This function creates a new polynom that is a copy of the given polynom.
 * dest must be a pointer to a valid polynom.
 * 
 * @param[in] src Polynom to copy.
 * @param[out] dest Pointer to the new polynom.
 * 
 * @see Polynomial_t
*/
void copy_polynom(Polynomial_t src, Polynomial_t* dest);

/**
 * @brief Delete a polynom
 * 
 * This function deletes the given polynom.
 * 
 * @param p Polynom to delete.
 * 
 * @see Polynomial_t
*/
void delete_polynom(Polynomial_t p);

/**
 * @brief Add two polynoms
 * 
 * This function adds two polynoms and returns the result.
 * Will use SSE2 instructions if available.
 * 
 * @param[in] p1 First polynom.
 * @param[in] p2 Second polynom.
 * @param[out] p Pointer to the result polynom.
 * 
 * @see Polynomial_t
*/
void add_polynoms(Polynomial_t p1, Polynomial_t p2, Polynomial_t* p);

/**
 * @brief Substract two polynoms
 * 
 * This function substracts two polynoms and returns the result.
 * Will use SSE2 instructions if available.
 * 
 * @param[in] p1 First polynom.
 * @param[in] p2 Second polynom.
 * @param[out] p Pointer to the result polynom.
 * 
 * @note In Z/2Z, addition and substraction are the same operation.
 * 
 * @see Polynomial_t
*/
void substract_polynoms(Polynomial_t p1, Polynomial_t p2, Polynomial_t* p);

/**
 * @brief Multiply two polynoms
 * 
 * This function multiplies two polynoms and returns the result.
 * Will use SSE2 instructions if available.
 * 
 * @param[in] p1 First polynom.
 * @param[in] p2 Second polynom.
 * @param[out] p Pointer to the result polynom.
 * 
 * @see Polynomial_t
*/
void multiply_polynoms(Polynomial_t p1, Polynomial_t p2, Polynomial_t* p);

/**
 * @brief Divide two polynoms
 * 
 * This function divides two polynoms and returns the quotient (euclidean division).
 * Will use SSE2 instructions if available.
 * 
 * @param[in] p1 First polynom.
 * @param[in] p2 Second polynom.
 * @param[out] p Pointer to the result polynom.
 * 
 * @see Polynomial_t
*/
void divide_polynoms(Polynomial_t p1, Polynomial_t p2, Polynomial_t* p);
