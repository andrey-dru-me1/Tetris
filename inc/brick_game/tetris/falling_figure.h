#ifndef FALLING_FIGURE_H_9451635
#define FALLING_FIGURE_H_9451635

#include <stdbool.h>

#include "brick_game/tetris/field.h"
#include "brick_game/tetris/figure.h"

typedef struct {
  figure_t *fig;
  field_t *field;
  int row;
  int col;
  int rotidx;
} falling_figure_t;

void falling_figure_create(falling_figure_t *ff, figure_t *fig, field_t *field, int row,
                int col, int rotidx);
bool falling_figure_rotate(falling_figure_t *ff);
bool falling_figure_shift(falling_figure_t *ff);
bool falling_figure_move(falling_figure_t *ff, int row, int col);
void falling_figure_commit(falling_figure_t *ff);
void falling_figure_remove(falling_figure_t *ff);

#endif
