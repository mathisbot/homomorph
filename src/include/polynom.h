#pragma once

#include <stdbool.h>
#include <stdint.h>

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
 * @brief Polynom structure and functions to manipulate polynoms.
 * 
 * This file contains the definition of the Polynom structure and functions to manipulate polynoms.
 * Polynoms are made to used in Z/2Z[X], the ring of polynoms with coefficients in Z/2Z.
 * 
 * @see Polynom
*/


/**
 * @brief Polynom structure
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
} Polynom;


/**
 * @brief Create a new polynom
 * 
 * This function creates a new polynom with the given degree. The coefficients of the polynom are initialized to false, expect for one of degree degree.
 * 
 * @param degree Degree of the polynom.
 * 
 * @return Polynom* Pointer to the new polynom.
 * 
 * @see Polynom
*/
Polynom* monom(pol_degree_t degree);

/**
 * @brief Return a constant polynom
 * 
 * This function returns a constant polynom with the given value.
 * 
 * @param value Value of the constant polynom.
 * 
 * @return Polynom* Pointer to the constant polynom.
 * 
 * @see Polynom
*/
Polynom* constant_polynom(bool value);

/**
 * @brief Create a random polynom
 * 
 * This function creates a new polynom with the given degree. The coefficients of the polynom are initialized to random values.
 * This function assumes that the random function has been seeded.
 * 
 * @param degree Degree of the polynom.
 * 
 * @return Polynom* Pointer to the new polynom.
 * 
 * @see Polynom
 * @see rand
*/
Polynom* random_polynom(pol_degree_t degree);

/**
 * @brief Create a copy of a polynom
 * 
 * This function creates a new polynom that is a copy of the given polynom.
 * 
 * @param p Pointer to the polynom to copy.
 * 
 * @return Polynom* Pointer to the new polynom.
 * 
 * @see Polynom
*/
Polynom* copy_polynom(Polynom* p);

/**
 * @brief Delete a polynom
 * 
 * This function deletes the given polynom.
 * 
 * @param p Pointer to the polynom to delete.
 * 
 * @see Polynom
*/
void delete_polynom(Polynom* p);

/**
 * @brief Add two polynoms
 * 
 * This function adds two polynoms and returns the result.
 * Will use SSE2 instructions if available.
 * 
 * @param p1 Pointer to the first polynom.
 * @param p2 Pointer to the second polynom.
 * 
 * @return Polynom* Pointer to the result polynom.
 * 
 * @see Polynom
*/
Polynom* add_polynoms(Polynom* p1, Polynom* p2);

/**
 * @brief Multiply two polynoms
 * 
 * This function multiplies two polynoms and returns the result.
 * Will use SSE2 instructions if available.
 * 
 * @param p1 Pointer to the first polynom.
 * @param p2 Pointer to the second polynom.
 * 
 * @return Polynom* Pointer to the result polynom.
 * 
 * @see Polynom
*/
Polynom* multiply_polynoms(Polynom* p1, Polynom* p2);

/**
 * @brief Divide two polynoms
 * 
 * This function divides two polynoms and returns the result (euclidean division).
 * Will use SSE2 instructions if available.
 * 
 * @param p1 Pointer to the first polynom.
 * @param p2 Pointer to the second polynom.
 * 
 * @return Polynom* Pointer to the result polynom.
 * 
 * @see Polynom
*/
Polynom* divide_polynoms(Polynom* p1, Polynom* p2);

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
 * @see Polynom
*/
bool evaluate_polynom(Polynom* p, bool x);

/**
 * @brief Print a polynom
 * 
 * This function prints a polynom to the standard output.
 * 
 * @param p Pointer to the polynom.
 * 
 * @see Polynom
*/
void print_polynom(Polynom* p);
