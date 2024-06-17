#include "brick_game/tetris/types/blockmatrix.h"

void blockmatrix_create(blockmatrix_t *blockmatrix, size_t rows, size_t cols) {
  blockarray_create(&blockmatrix->ba, rows * cols);
  blockmatrix->rows = rows;
  blockmatrix->cols = cols;
}

block_t blockmatrix_get(blockmatrix_t *blockmatrix, size_t row, size_t col) {
  return blockarray_get(&blockmatrix->ba, row * blockmatrix->cols + col);
}

void blockmatrix_set(blockmatrix_t *blockmatrix, size_t row, size_t col,
                       block_t val) {
  blockarray_set(&blockmatrix->ba, row * blockmatrix->cols + col, val);
}

void blockmatrix_remove(blockmatrix_t *blockmatrix) {
  blockarray_remove(&blockmatrix->ba);
}

blockmatrix_t blockmatrix_rotate(blockmatrix_t *blockmatrix) {
  blockmatrix_t newmtx;
  blockmatrix_create(&newmtx, blockmatrix->cols, blockmatrix->rows);
  for (int i = 0; i < blockmatrix->rows; i++) {
    for (int j = 0; j < blockmatrix->cols; j++) {
      blockmatrix_set(&newmtx, j, newmtx.cols - 1 - i,
                        blockmatrix_get(blockmatrix, i, j));
    }
  }
  return newmtx;
}

blockmatrix_t blockmatrix_flip_vertically(blockmatrix_t *blockmatrix) {
  blockmatrix_t newmtx;
  blockmatrix_create(&newmtx, blockmatrix->rows, blockmatrix->cols);
  for (int i = 0; i < blockmatrix->rows; i++) {
    for (int j = 0; j < blockmatrix->cols; j++) {
      blockmatrix_set(&newmtx, i, newmtx.cols - 1 - j,
                        blockmatrix_get(blockmatrix, i, j));
    }
  }
  return newmtx;
}
