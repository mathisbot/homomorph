#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <emmintrin.h>

#include "utils.h"

#ifdef __AVX2__
#define SSE2_SUPPORTED 1
#define AVX2_VECTOR_SIZE 32  // 256 bits = 32 bytes
#else
#define SSE2_SUPPORTED 0
#endif


#define pol_degree_t uint32_t
#define MAX_POLYNOM_DEGREE 4294967295


/**
 * @file polynom.h
 * @brief Polynomial_t structure and functions to manipulate polynoms.
 * 
 * This file contains the definition of the Polynomial_t structure and functions to manipulate polynoms.
 * Polynoms are made to used in Z/2Z[X], the ring of polynoms with coefficients in Z/2Z.
 * 
 * @see Polynomial_t
*/


/**
 * @brief Polynomial_t structure
 * 
 * This structure is used to represent a polynom. It contains a pointer to an array of booleans, which represents the coefficients of the polynom, and the degree of the polynom.
 * Degree is the number of coefficients minus one, NOT the highest degree of the polynom.
 * 
 * @param coefficients Pointer to an array of booleans, which represents the coefficients of the polynom.
 * @param degree Degree of the polynom.
 * 
 * @see bool
 * @see pol_degree_t
*/
typedef struct {
    bool* coefficients;
    pol_degree_t degree;
} Polynomial_t;


/**
 * @brief Create a new polynom
 * 
 * This function creates a new polynom with the given degree. The coefficients of the polynom are initialized to false, expect for one of degree degree.
 * 
 * @param[in] degree Degree of the polynom.
 * @param[out] p Pointer to the new polynom.
 * 
 * @see Polynomial_t
*/
void monom(pol_degree_t degree, Polynomial_t* p);

/**
 * @brief Return a constant polynom
 * 
 * This function returns a constant polynom with the given value.
 * 
 * @param[in] value Value of the constant polynom.
 * @param[out] p Pointer to the new polynom.
 *  
 * @see Polynomial_t
*/
void constant_polynom(bool value, Polynomial_t* p);

/**
 * @brief Create a random polynom
 * 
 * This function creates a new polynom with the given degree. The coefficients of the polynom are initialized to random values.
 * This function assumes that the random function has been seeded.
 * 
 * @param[in] degree Degree of the polynom.
 * @param[out] p Pointer to the new polynom.
 * 
 * @see Polynomial_t
 * @see rand
*/
void random_polynom(pol_degree_t degree, Polynomial_t* p);

/**
 * @brief Create a copy of a polynom
 * 
 * This function creates a new polynom that is a copy of the given polynom.
 * 
 * @param[in] src Pointer to the polynom to copy.
 * @param[out] dest Pointer to the new polynom.
 * 
 * @see Polynomial_t
*/
void copy_polynom(Polynomial_t* src, Polynomial_t* dest);

/**
 * @brief Delete a polynom
 * 
 * This function deletes the given polynom.
 * 
 * @param p Pointer to the polynom to delete.
 * 
 * @see Polynomial_t
*/
void delete_polynom(Polynomial_t* p);

/**
 * @brief Add two polynoms
 * 
 * This function adds two polynoms and returns the result.
 * Will use SSE2 instructions if available.
 * 
 * @param[in] p1 Pointer to the first polynom.
 * @param[in] p2 Pointer to the second polynom.
 * @param[out] p Pointer to the result polynom.
 * 
 * @see Polynomial_t
*/
void add_polynoms(Polynomial_t* p1, Polynomial_t* p2, Polynomial_t* p);

/**
 * @brief Multiply two polynoms
 * 
 * This function multiplies two polynoms and returns the result.
 * Will use SSE2 instructions if available.
 * 
 * @param p1 Pointer to the first polynom.
 * @param p2 Pointer to the second polynom.
 * @param[out] p Pointer to the result polynom.
 * 
 * @see Polynomial_t
*/
void multiply_polynoms(Polynomial_t* p1, Polynomial_t* p2, Polynomial_t* p);

/**
 * @brief Divide two polynoms
 * 
 * This function divides two polynoms and returns the result (euclidean division).
 * Will use SSE2 instructions if available.
 * 
 * @param p1 Pointer to the first polynom.
 * @param p2 Pointer to the second polynom.
 * @param[out] p Pointer to the result polynom.
 * 
 * @see Polynomial_t
*/
void divide_polynoms(Polynomial_t* p1, Polynomial_t* p2, Polynomial_t* p);

/**
 * @brief Evaluate a polynom
 * 
 * This function evaluates a polynom at a given point and returns the result.
 * 
 * @param p Pointer to the polynom.
 * @param x Point to evaluate the polynom at.
 * 
 * @return bool Result of the evaluation.
 * 
 * @see Polynomial_t
*/
bool evaluate_polynom(Polynomial_t* p, bool x);
