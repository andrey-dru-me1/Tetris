#ifndef FALLING_FIGURE_H_9451635
#define FALLING_FIGURE_H_9451635

#include <stdbool.h>

#include "brick_game/tetris/figure.h"

typedef struct {
  figure_t *fig;
  int row;
  int col;
  int rotidx;
} falling_figure_t;

#endif
