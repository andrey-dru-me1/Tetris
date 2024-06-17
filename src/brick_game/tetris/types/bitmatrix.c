#include "brick_game/tetris/types/bitmatrix.h"

void bitmatrix_create(bitmatrix_t *bitmatrix, size_t rows, size_t cols) {
  bitarray_create(&bitmatrix->ba, rows * cols);
  bitmatrix->rows = rows;
  bitmatrix->cols = cols;
}

bit_t bitmatrix_get(bitmatrix_t *bitmatrix, size_t row, size_t col) {
  return bitarray_get(&bitmatrix->ba, row * bitmatrix->cols + col);
}

void bitmatrix_set(bitmatrix_t *bitmatrix, size_t row, size_t col) {
  bitarray_set_bit(&bitmatrix->ba, row * bitmatrix->cols + col, 1);
}

void bitmatrix_reset(bitmatrix_t *bitmatrix, size_t row, size_t col) {
  bitarray_set_bit(&bitmatrix->ba, row * bitmatrix->cols + col, 0);
}

void bitmatrix_set_bit(bitmatrix_t *bitmatrix, size_t row, size_t col,
                       bit_t val) {
  bitarray_set_bit(&bitmatrix->ba, row * bitmatrix->cols + col, val);
}

void bitmatrix_remove(bitmatrix_t *bitmatrix) {
  bitarray_remove(&bitmatrix->ba);
}

bitmatrix_t bitmatrix_rotate(bitmatrix_t *bitmatrix) {
  bitmatrix_t newmtx;
  bitmatrix_create(&newmtx, bitmatrix->cols, bitmatrix->rows);
  for (int i = 0; i < bitmatrix->rows; i++) {
    for (int j = 0; j < bitmatrix->cols; j++) {
      bitmatrix_set_bit(&newmtx, j, newmtx.cols - 1 - i,
                        bitmatrix_get(bitmatrix, i, j));
    }
  }
  return newmtx;
}

bitmatrix_t bitmatrix_flip_vertically(bitmatrix_t *bitmatrix) {
  bitmatrix_t newmtx;
  bitmatrix_create(&newmtx, bitmatrix->rows, bitmatrix->cols);
  for (int i = 0; i < bitmatrix->rows; i++) {
    for (int j = 0; j < bitmatrix->cols; j++) {
      bitmatrix_set_bit(&newmtx, i, newmtx.cols - 1 - j,
                        bitmatrix_get(bitmatrix, i, j));
    }
  }
  return newmtx;
}
