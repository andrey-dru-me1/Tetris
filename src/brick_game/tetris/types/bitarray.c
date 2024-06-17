#include "brick_game/tetris/types/bitarray.h"

#include <stdlib.h>

void bitarray_create(bitarray_t *bitarray, size_t n) {
  bitarray->bits = calloc(n, sizeof(bit_t));
  bitarray->n = n;
}

bit_t bitarray_get(bitarray_t *bitarray, size_t i) {
  return bitarray->bits[i];
}

void bitarray_set(bitarray_t *bitarray, size_t i) {
  bitarray->bits[i] = 1;
}

void bitarray_reset(bitarray_t *bitarray, size_t i) {
  bitarray->bits[i] = 0;
}

void bitarray_set_bit(bitarray_t *bitarray, size_t i, bit_t val) {
  bitarray->bits[i] = val;
}

void bitarray_remove(bitarray_t *bitarray) { free(bitarray->bits); }
