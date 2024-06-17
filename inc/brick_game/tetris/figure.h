#ifndef FIGURE_H_684651
#define FIGURE_H_684651

#include "brick_game/tetris/types/blockmatrix.h"

/**
 * `figure_t` type contains array of rotations and length of this array. Each
 * rotation is a blockmatrix of preset sizes, where 1 is stands for brick and 0
 * stands for emptiness.
 */
typedef struct {
  blockmatrix_t *rots;
  size_t rotcnt;
} figure_t;

void figure_create(figure_t *figure, size_t rows, size_t cols, size_t rotcnt);
blockmatrix_t figure_get(figure_t *figure, size_t i);
void figure_set(figure_t *figure, size_t i, blockmatrix_t val);
void figure_remove(figure_t *figure);

#endif
