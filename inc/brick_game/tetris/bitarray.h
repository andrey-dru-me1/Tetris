#ifndef BITARRAY_H_68415
#define BITARRAY_H_68415

#include <stddef.h>

#include "brick_game/tetris/bit.h"

typedef struct {
  bit_t *val;
  size_t n;
} bitarray_t;

void bitarray_create(bitarray_t *bitarray, size_t n);
bit_t bitarray_get(bitarray_t *bitarray, size_t i);
void bitarray_set(bitarray_t *bitarray, size_t i);
void bitarray_reset(bitarray_t *bitarray, size_t i);
void bitarray_set_bit(bitarray_t *bitarray, size_t i, bit_t val);
void bitarray_remove(bitarray_t *bitarray);

#endif
