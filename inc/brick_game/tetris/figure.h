#ifndef FIGURE_H_684651
#define FIGURE_H_684651

#include "brick_game/tetris/types/blockmatrix.h"

typedef enum { I = 1, L, J, O, T, S, Z } figtype_t;

/**
 * `figure_t` type contains array of rotations and length of this array. Each
 * rotation is a blockmatrix of preset sizes, where 1 is stands for brick and 0
 * stands for emptiness.
 */
typedef struct {
  blockmatrix_t *rots;
  size_t rotcnt;
  int rowsempty;
  figtype_t figtype;
} figure_t;

void figure_create(figure_t *figure, size_t rows, size_t cols, size_t rotcnt,
                   int rowsempty, figtype_t figtype);
void figure_createfromarray(figure_t *figure, size_t rows, size_t cols,
                            size_t rotcnt, figtype_t figtype, block_t arr[]);
blockmatrix_t figure_get(figure_t *figure, size_t i);
void figure_remove(figure_t *figure);

#endif
