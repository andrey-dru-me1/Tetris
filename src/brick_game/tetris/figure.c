#include "brick_game/tetris/figure.h"

#include <stdlib.h>
#include <string.h>

void figure_create(figure_t *figure, size_t rows, size_t cols, size_t rotcnt,
                   figtype_t figtype) {
  figure->rots = calloc(rotcnt, sizeof(blockmatrix_t));
  for (size_t i = 0; i < rotcnt; i++) {
    blockmatrix_create(figure->rots + i, rows, cols);
  }
  figure->rotcnt = rotcnt;
  figure->figtype = figtype;
}

blockmatrix_t figure_get(figure_t *figure, size_t i) { return figure->rots[i]; }

void figure_remove(figure_t *figure) {
  for (size_t i = 0; i < figure->rotcnt; i++) {
    blockmatrix_remove(figure->rots + i);
  }
  free(figure->rots);
}

void figure_createfromarray(figure_t *figure, size_t rows, size_t cols,
                            size_t rotcnt, figtype_t figtype, block_t arr[]) {
  figure_create(figure, rows, cols, rotcnt, figtype);
  memmove(figure->rots->blocks + 0, arr, rows * cols * sizeof(block_t));
  for (size_t i = 1; i < rotcnt; i++)
    blockmatrix_rotate(figure->rots + i - 1, figure->rots + i);
}
