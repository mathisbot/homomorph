#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))

typedef struct {
    bool* elements;
    uint64_t size;
} Part;

Part* random_part(uint64_t size);