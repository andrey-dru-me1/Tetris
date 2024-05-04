#ifndef BITARRAY_H_68415
#define BITARRAY_H_68415

#include <stddef.h>

#include "brick_game/tetris/bit.h"

typedef struct {
  bit *val;
  size_t n;
} bitarray_t;

struct bitarray_module {
  void (*create)(bitarray_t *bitarray, size_t n);
  bit (*get)(bitarray_t *bitarray, size_t i);
  void (*set)(bitarray_t *bitarray, size_t i);
  void (*reset)(bitarray_t *bitarray, size_t i);
  void (*set_bit)(bitarray_t *bitarray, size_t i, bit val);
  void (*remove)(bitarray_t *bitarray);
};

extern struct bitarray_module bitarray;

#endif
