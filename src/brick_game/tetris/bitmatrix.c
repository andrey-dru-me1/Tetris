#include "brick_game/tetris/bitmatrix.h"

static void create(bitmatrix_t *bitmatrix, size_t rows, size_t cols) {
  bitarray.create(&bitmatrix->val, rows * cols);
  bitmatrix->rows = rows;
  bitmatrix->cols = cols;
}

static bit get(bitmatrix_t *bitmatrix, size_t row, size_t col) {
  return bitarray.get(&bitmatrix->val, row * bitmatrix->cols + col);
}

static void set(bitmatrix_t *bitmatrix, size_t row, size_t col) {
  bitarray.set(&bitmatrix->val, row * bitmatrix->cols + col);
}

static void reset(bitmatrix_t *bitmatrix, size_t row, size_t col) {
  bitarray.reset(&bitmatrix->val, row * bitmatrix->cols + col);
}

static void set_bit(bitmatrix_t *bitmatrix, size_t row, size_t col, bit val) {
  bitarray.set_bit(&bitmatrix->val, row * bitmatrix->cols + col, val);
}

static void remove(bitmatrix_t *bitmatrix) { bitarray.remove(&bitmatrix->val); }

static bitmatrix_t rotate(bitmatrix_t *bitmatrix) {
  bitmatrix_t newmtx;
  create(&newmtx, bitmatrix->cols, bitmatrix->rows);
  for (int i = 0; i < bitmatrix->rows; i++) {
    for (int j = 0; j < bitmatrix->cols; j++) {
      set_bit(&newmtx, j, newmtx.cols - 1 - i, get(bitmatrix, i, j));
    }
  }
  return newmtx;
}

static bitmatrix_t flip_vertically(bitmatrix_t *bitmatrix) {
  bitmatrix_t newmtx;
  create(&newmtx, bitmatrix->rows, bitmatrix->cols);
  for (int i = 0; i < bitmatrix->rows; i++) {
    for (int j = 0; j < bitmatrix->cols; j++) {
      set_bit(&newmtx, i, newmtx.cols - 1 - j, get(bitmatrix, i, j));
    }
  }
  return newmtx;
}

struct bitmatrix_module bitmatrix = {.create = create,
                                     .get = get,
                                     .set = set,
                                     .reset = reset,
                                     .set_bit = set_bit,
                                     .remove = remove,
                                     .rotate = rotate,
                                     .flip_vertically = flip_vertically};
