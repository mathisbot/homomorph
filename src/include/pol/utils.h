#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdio.h> // debug

#define MAX(x, y) ((x) > (y) ? (x) : (y))

typedef struct {
    bool* elements;
    uint64_t size;
} Part;

/**
 * @brief Create a new part
 * 
 * This function creates a new part with the given size. The elements of the part are initialized to random values.
 * 
 * @param[in] size Size of the part.
 * @return Pointer to the new part.
 * 
 * @see Part
*/
Part random_part(uint64_t size);

/**
 * @brief Delete a part
 * 
 * This function deletes a part and frees the memory allocated for it.
 * 
 * @param[in] part Pointer to the part to delete.
 * 
 * @see Part
*/
void delete_part(Part part);