#include "brick_game/tetris/figure.h"

#include <stdlib.h>

void figure_create(figure_t *figure, size_t rotcnt) {
  figure->rots = calloc(rotcnt, sizeof(bitmatrix_t));
  figure->rotcnt = rotcnt;
}

bitmatrix_t figure_get(figure_t *figure, size_t i) { return figure->rots[i]; }

void figure_set(figure_t *figure, size_t i, bitmatrix_t bitmatrix) {
  figure->rots[i] = bitmatrix;
}

void figure_remove(figure_t *figure) {
  for (size_t i = 0; i < figure->rotcnt; i++) {
    bitmatrix_remove(figure->rots + i);
  }
  free(figure->rots);
}
