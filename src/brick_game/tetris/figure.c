#include "brick_game/tetris/figure.h"

#include <stdlib.h>

void figure_create(figure_t *figure, size_t rows, size_t cols, size_t rotcnt) {
  figure->rots = calloc(rotcnt, sizeof(blockmatrix_t));
  for (size_t i = 0; i < rotcnt; i++) {
    blockmatrix_create(figure->rots + i, rows, cols);
  }
  figure->rotcnt = rotcnt;
}

blockmatrix_t figure_get(figure_t *figure, size_t i) { return figure->rots[i]; }

void figure_set(figure_t *figure, size_t i, blockmatrix_t blockmatrix) {
  figure->rots[i] = blockmatrix;
}

void figure_remove(figure_t *figure) {
  for (size_t i = 0; i < figure->rotcnt; i++) {
    blockmatrix_remove(figure->rots + i);
  }
  free(figure->rots);
}
