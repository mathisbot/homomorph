#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// TODO: remove
#include <stdio.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/**
 * @file utils.h
 * @brief Part structure and functions to manipulate parts.
 * 
 * This file contains the definition of the Part structure and functions to manipulate parts.
 * Parts are subsets of [1,n], represented by a boolean array of size n.
 * 
 * @see Part
*/

/**
 * @brief Part structure
 * 
 * This structure is used to represent a part. It contains a pointer to an array of booleans, which represents the elements of the part, and the size of the part.
 * 
 * @param elements Pointer to an array of booleans, which represents the elements of the part.
 * @param size Size of the part.
 * 
 * @example
 * // This means that the part contains the first, third and fifth elements of the set [1,5]
 * part.size = 5
 * part.elements = [true, false, true, false, true]
 * 
 * @see bool
*/
typedef struct {
    bool* elements;
    uint64_t size;
} Part;

/**
 * @brief Create a new part
 * 
 * This function creates a new part of [1,size].
 * The elements of the part are initialized to random values.
 * 
 * @param[in] size Size of the part.
 * @return New random part.
 * 
 * @note The random function must be seeded before calling this function.
 * 
 * @see Part
*/
Part random_part(uint64_t size);

/**
 * @brief Delete a part
 * 
 * This function deletes a part and frees the memory allocated for it.
 * 
 * @param[in] part Part to delete.
 * 
 * @see Part
*/
void delete_part(Part part);