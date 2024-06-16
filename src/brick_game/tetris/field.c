#include "brick_game/tetris/field.h"

static bool _field_validatefig(field_t *f) {
  bool does_overlap = false;

  bitmatrix_t bm = figure_get(f->ff.fig, f->ff.rotidx);
  for (int i = 0; !does_overlap && i < bm.rows; i++) {
    for (int j = 0; !does_overlap && j < bm.cols; j++) {
      bit_t figbit = bitmatrix_get(&bm, i, j);
      if (figbit == 1 &&
          (f->ff.row + i < 0 || f->ff.row + i >= f->bm.rows ||
           f->ff.col + j < 0 || f->ff.col + j >= f->bm.cols ||
           bitmatrix_get(&f->bm, f->ff.row + i, f->ff.col + j) == 1))
        does_overlap = true;
    }
  }

  return !does_overlap;
}

bool field_spawnfig(field_t *f, figure_t *fig, int row, int col, int rotidx) {
  f->ff =
      (falling_figure_t){.fig = fig, .row = row, .col = col, .rotidx = rotidx};
  return _field_validatefig(f);
}

bool field_rotatefig(field_t *f) {
  int prev_rotidx = f->ff.rotidx;

  bool validfig;
  do {
    f->ff.rotidx = (f->ff.rotidx + 1) % f->ff.fig->rotcnt;
    validfig = _field_validatefig(f);
  } while (!validfig);

  return f->ff.rotidx != prev_rotidx;
}

bool field_movefig(field_t *f, int row, int col) {
  int prev_row = f->ff.row, prev_col = f->ff.col;
  f->ff.row = row;
  f->ff.col = col;

  bool validfig = _field_validatefig(f);
  if (!validfig) {
    f->ff.row = prev_row;
    f->ff.col = prev_col;
  }

  return validfig;
}

bool field_shiftfig(field_t *f) {
  return field_movefig(f, f->ff.row + 1, f->ff.col);
}

void field_commitfig(field_t *f) {
  bitmatrix_t bm = figure_get(f->ff.fig, f->ff.rotidx);
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (bitmatrix_get(&bm, i, j))
        bitmatrix_set(&f->bm, f->ff.row + i, f->ff.col + j);
    }
  }
}

void field_removefig(field_t *f) {
  // Nothing to be freed: falling figure does not *own* anything
}

static void _field_dropline(field_t *f, size_t droprow) {
  for (size_t row = droprow; row > 0; row--) {
    for (size_t col = 0; col < f->bm.cols; col++) {
      bitmatrix_set_bit(&f->bm, row, col, bitmatrix_get(&f->bm, row - 1, col));
    }
  }
}

int field_droplines(field_t *f) {
  int dropcount = 0;
  for (size_t row = 0; row < f->bm.rows; row++) {
    bool linefilled = true;
    for (size_t col = 0;
         col < f->bm.cols && (linefilled = bitmatrix_get(&f->bm, row, col));
         col++);
    if (linefilled) {
      _field_dropline(f, row);
      dropcount++;
    }
  }
  return dropcount;
}
