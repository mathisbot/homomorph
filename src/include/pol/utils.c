#include "utils.h"

Part random_part(uint64_t size) {
    Part part = {0};
    part.size = size;
    part.elements = (bool*) malloc(size * sizeof(bool));
    if (part.elements == NULL) exit(1);
    for (uint64_t i = 0; i < size; i++) {
        part.elements[i] = rand() % 2;
    }
    return part;
}

void delete_part(Part part) {
    free(part.elements);
}