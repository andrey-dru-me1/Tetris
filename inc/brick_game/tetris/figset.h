#ifndef FIGGEN_H_34987486
#define FIGGEN_H_34987486

#include "brick_game/tetris/figure.h"

typedef struct {
  figure_t figs[7];
} figset_t;

void figset_init(figset_t *figset);
void figset_free(figset_t *figset);

#endif
