#include "brick_game/tetris/bitarray.h"

#include <stdlib.h>

static void create(bitarray_t *bitarray, size_t n) {
  size_t actual_len = (n - 1) / 8 + 1;
  bitarray->val = calloc(actual_len, sizeof(bit));
  bitarray->n = n;
}

static bit get(bitarray_t *bitarray, size_t i) {
  return (bitarray->val[i / 8] >> (i % 8)) & 0b1u;
}

static void set(bitarray_t *bitarray, size_t i) {
  bitarray->val[i / 8] |= 1u << (i % 8);
}

static void reset(bitarray_t *bitarray, size_t i) {
  bitarray->val[i / 8] &= ~(1u << (i % 8));
}

static void set_bit(bitarray_t *bitarray, size_t i, bit val) {
  if (val == 1)
    set(bitarray, i);
  else if (val == 0)
    reset(bitarray, i);
}

static void remove(bitarray_t *bitarray) { free(bitarray->val); }

struct bitarray_module bitarray = {.create = create,
                                   .get = get,
                                   .set = set,
                                   .reset = reset,
                                   .set_bit = set_bit,
                                   .remove = remove};
