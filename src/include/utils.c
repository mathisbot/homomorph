#include "utils.h"

Part* random_part(uint64_t size) {
    Part* part = (Part*) malloc(sizeof(Part));
    if (part == NULL) return NULL;
    part->elements = (bool*) malloc(size * sizeof(bool));
    if (part->elements == NULL) { free(part); return NULL; }
    for (uint64_t i = 0; i < size; i++) {
        part->elements[i] = rand() % 2;
    }
    part->size = size;
    return part;
}