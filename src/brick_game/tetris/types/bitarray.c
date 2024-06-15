#include "brick_game/tetris/types/bitarray.h"

#include <stdlib.h>

void bitarray_create(bitarray_t *bitarray, size_t n) {
  size_t actual_len = (n - 1) / 8 + 1;
  bitarray->bits = calloc(actual_len, sizeof(bit_t));
  bitarray->n = n;
}

bit_t bitarray_get(bitarray_t *bitarray, size_t i) {
  return (bitarray->bits[i / 8] >> (i % 8)) & 0b1u;
}

void bitarray_set(bitarray_t *bitarray, size_t i) {
  bitarray->bits[i / 8] |= 1u << (i % 8);
}

void bitarray_reset(bitarray_t *bitarray, size_t i) {
  bitarray->bits[i / 8] &= ~(1u << (i % 8));
}

void bitarray_set_bit(bitarray_t *bitarray, size_t i, bit_t val) {
  if (val == 1)
    bitarray_set(bitarray, i);
  else if (val == 0)
    bitarray_reset(bitarray, i);
}

void bitarray_remove(bitarray_t *bitarray) { free(bitarray->bits); }
