#ifndef BITMATRIX_H_78651
#define BITMATRIX_H_78651

#include "brick_game/tetris/types/bitarray.h"

typedef struct {
  bitarray_t ba;
  size_t rows;
  size_t cols;
} bitmatrix_t;

void bitmatrix_create(bitmatrix_t *bitmatrix, size_t rows, size_t cols);
bit_t bitmatrix_get(bitmatrix_t *bitmatrix, size_t row, size_t col);
void bitmatrix_set_bit(bitmatrix_t *bitmatrix, size_t row, size_t col,
                       bit_t val);
void bitmatrix_remove(bitmatrix_t *bitmatrix);
bitmatrix_t bitmatrix_rotate(bitmatrix_t *bitmatrix);
bitmatrix_t bitmatrix_flip_vertically(bitmatrix_t *bitmatrix);

#endif
