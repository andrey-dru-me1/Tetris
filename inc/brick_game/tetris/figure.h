#ifndef FIGURE_H_684651
#define FIGURE_H_684651

#include "bitmatrix.h"

/**
 * `figure_t` type contains array of rotations and length of this array. Each
 * rotation is a bitmatrix of preset sizes, where 1 is stands for brick and 0
 * stands for emptiness.
 */
typedef struct {
  bitmatrix_t *rots;
  size_t rotcnt;
} figure_t;

struct figure_module {
  void (*create)(figure_t *figure, size_t rotcnt);
  bitmatrix_t (*get)(figure_t *figure, size_t i);
  void (*set)(figure_t *figure, size_t i, bitmatrix_t val);
  void (*remove)(figure_t *figure);
};

extern struct figure_module figure;

#endif
