#ifndef BITMATRIX_H_78651
#define BITMATRIX_H_78651

#include "brick_game/tetris/types/block.h"

#include <stddef.h>

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
blockmatrix_t blockmatrix_rotate(blockmatrix_t *blockmatrix);
blockmatrix_t blockmatrix_flip_vertically(blockmatrix_t *blockmatrix);

#endif
