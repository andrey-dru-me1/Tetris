#ifndef BITMATRIX_H_78651
#define BITMATRIX_H_78651

#include <stddef.h>

#include "brick_game/tetris/types/block.h"

typedef struct {
  block_t *blocks;
  size_t rows;
  size_t cols;
} blockmatrix_t;

void blockmatrix_create(blockmatrix_t *blockmatrix, size_t rows, size_t cols);
block_t blockmatrix_get(blockmatrix_t *blockmatrix, size_t row, size_t col);
void blockmatrix_set(blockmatrix_t *blockmatrix, size_t row, size_t col,
                     block_t val);
void blockmatrix_remove(blockmatrix_t *blockmatrix);
void blockmatrix_rotate(blockmatrix_t *src, blockmatrix_t *dst);
void blockmatrix_copy(blockmatrix_t *src, blockmatrix_t *dst);

#endif
