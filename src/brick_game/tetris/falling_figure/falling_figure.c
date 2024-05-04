#include "falling_figure.h"

static void create(falling_figure_t *ff, figure_t *fig, field_t *field, int row,
                   int col, int rotidx) {
  *ff = (falling_figure_t){
      .fig = fig, .field = field, .row = row, .col = col, .rotidx = rotidx};
}

static bool validate(falling_figure_t *ff) {
  bool does_overlap = false;

  bitmatrix_t bm = figure.get(ff->fig, ff->rotidx);
  for (int i = 0; !does_overlap && i < bm.rows; i++) {
    for (int j = 0; !does_overlap && j < bm.cols; j++) {
      bit figbit = bitmatrix.get(&bm, i, j);
      if (figbit == 1 &&
          (ff->row + i < 0 || ff->row + i >= ff->field->rows ||
           ff->col + j < 0 || ff->col + j >= ff->field->cols ||
           bitmatrix.get(ff->field, ff->row + i, ff->col + j) == 1))
        does_overlap = true;
    }
  }

  return !does_overlap;
}

static bool rotate(falling_figure_t *ff) {
  int prev_rotidx = ff->rotidx;

  bool validfig;
  do {
    ff->rotidx = (ff->rotidx + 1) % ff->fig->rotcnt;
    validfig = validate(ff);
  } while (!validfig);

  return ff->rotidx != prev_rotidx;
}

static bool move(falling_figure_t *ff, int row, int col) {
  int prev_row = ff->row, prev_col = ff->col;
  ff->row = row;
  ff->col = col;

  bool validfig = validate(ff);
  if (!validfig) {
    ff->row = prev_row;
    ff->col = prev_col;
  }

  return validfig;
}

static bool shift(falling_figure_t *ff) {
  return move(ff, ff->row + 1, ff->col);
}

static void commit(falling_figure_t *ff) {
  bitmatrix_t bm = figure.get(ff->fig, ff->rotidx);
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (bitmatrix.get(&bm, i, j))
        bitmatrix.set(ff->field, ff->row + i, ff->col + j);
    }
  }
}

static void remove(falling_figure_t *ff) {
  // Nothing to be freed: falling figure does not *own* anything
}

struct falling_figure_module falling_figure = {.create = create,
                                               .commit = commit,
                                               .move = move,
                                               .rotate = rotate,
                                               .shift = shift,
                                               .remove = remove};
