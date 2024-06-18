#include "brick_game/tetris/types/blockmatrix.h"

#include <stdlib.h>
#include <string.h>

void blockmatrix_create(blockmatrix_t *blockmatrix, size_t rows, size_t cols) {
  blockmatrix->blocks = calloc(rows * cols, sizeof(*blockmatrix->blocks));
  blockmatrix->rows = rows;
  blockmatrix->cols = cols;
}

block_t blockmatrix_get(blockmatrix_t *blockmatrix, size_t row, size_t col) {
  return blockmatrix->blocks[row * blockmatrix->cols + col];
}

void blockmatrix_set(blockmatrix_t *blockmatrix, size_t row, size_t col,
                     block_t val) {
  blockmatrix->blocks[row * blockmatrix->cols + col] = val;
}

void blockmatrix_remove(blockmatrix_t *blockmatrix) {
  free(blockmatrix->blocks);
}

void blockmatrix_rotate(blockmatrix_t *src, blockmatrix_t *dst) {
  blockmatrix_t new;
  blockmatrix_create(&new, src->cols, src->rows);
  for (int i = 0; i < src->rows; i++) {
    for (int j = 0; j < src->cols; j++) {
      blockmatrix_set(&new, j, src->rows - 1 - i, blockmatrix_get(src, i, j));
    }
  }
  blockmatrix_copy(&new, dst);
  blockmatrix_remove(&new);
}

void blockmatrix_copy(blockmatrix_t *src, blockmatrix_t *dst) {
  dst->rows = src->rows;
  dst->cols = src->cols;
  memmove(dst->blocks, src->blocks,
          src->rows * src->cols * sizeof(*src->blocks));
}
