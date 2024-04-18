#ifndef BITMATRIX_H_78651
#define BITMATRIX_H_78651

#include "../bitarray/bitarray.h"

typedef struct {
  bitarray_t val;
  size_t rows;
  size_t cols;
} bitmatrix_t;

struct bitmatrix_module {
  void (*create)(bitmatrix_t *bitmatrix, size_t rows, size_t cols);
  bit (*get)(bitmatrix_t *bitmatrix, size_t row, size_t col);
  void (*set)(bitmatrix_t *bitmatrix, size_t row, size_t col);
  void (*reset)(bitmatrix_t *bitmatrix, size_t row, size_t col);
  void (*set_bit)(bitmatrix_t *bitmatrix, size_t row, size_t col, bit val);
  void (*remove)(bitmatrix_t *bitmatrix);
  bitmatrix_t (*rotate)(bitmatrix_t *bitmatrix);
  bitmatrix_t (*flip_vertically)(bitmatrix_t *bitmatrix);
};

extern struct bitmatrix_module bitmatrix;

#endif
