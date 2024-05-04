#include "brick_game/tetris/figure.h"

#include <stdlib.h>

static void create(figure_t *figure, size_t rotcnt) {
  figure->rots = calloc(rotcnt, sizeof(bitmatrix_t));
  figure->rotcnt = rotcnt;
}

static bitmatrix_t get(figure_t *figure, size_t i) { return figure->rots[i]; }

static void set(figure_t *figure, size_t i, bitmatrix_t bitmatrix) {
  figure->rots[i] = bitmatrix;
}

static void remove(figure_t *figure) {
  for (size_t i = 0; i < figure->rotcnt; i++) {
    bitmatrix.remove(figure->rots + i);
  }
  free(figure->rots);
}

struct figure_module figure = {
    .create = create, .get = get, .set = set, .remove = remove};
