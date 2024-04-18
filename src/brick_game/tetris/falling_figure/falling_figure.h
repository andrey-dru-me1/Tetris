#ifndef FALLING_FIGURE_H_9451635
#define FALLING_FIGURE_H_9451635

#include <stdbool.h>

#include "../field.h"
#include "../figure/figure.h"

typedef struct {
  figure_t *fig;
  field_t *field;
  int row;
  int col;
  int rotidx;
} falling_figure_t;

struct falling_figure_module {
  void (*create)(falling_figure_t *ff, figure_t *fig, field_t *field, int row,
                 int col, int rotidx);
  bool (*rotate)(falling_figure_t *ff);
  bool (*shift)(falling_figure_t *ff);
  bool (*move)(falling_figure_t *ff, int row, int col);
  void (*commit)(falling_figure_t *ff);
  void (*remove)(falling_figure_t *ff);
};

extern struct falling_figure_module falling_figure;

#endif
