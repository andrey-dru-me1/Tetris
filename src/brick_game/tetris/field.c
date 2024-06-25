#include "brick_game/tetris/field.h"

enum overlap_e { Ok, Fig, Border };

static enum overlap_e _field_validatefig(field_t *f) {
  enum overlap_e overlap = Ok;

  blockmatrix_t bm = figure_get(f->ff.fig, f->ff.rotidx);
  for (int i = 0; overlap == Ok && i < bm.rows; i++) {
    for (int j = 0; overlap == Ok && j < bm.cols; j++) {
      block_t figblock = blockmatrix_get(&bm, i, j);
      if (figblock != 0 && (f->ff.col + j < 0 || f->ff.col + j >= f->bm.cols)) {
        overlap = Border;
      } else if (figblock != 0 && (f->ff.row + i >= (int)f->bm.rows ||
                                   (f->ff.row + i >= 0 &&
                                    blockmatrix_get(&f->bm, f->ff.row + i,
                                                    f->ff.col + j) != 0))) {
        overlap = Fig;
      }
    }
  }

  return overlap;
}

bool field_spawnfig(field_t *f, figure_t *fig, int row, int col, int rotidx) {
  f->ff =
      (falling_figure_t){.fig = fig, .row = row, .col = col, .rotidx = rotidx};
  return _field_validatefig(f) == Ok;
}

bool field_rotatefig(field_t *f) {
  int prev_rotidx = f->ff.rotidx;

  enum overlap_e validfig;
  do {
    f->ff.rotidx = (f->ff.rotidx + 1) % f->ff.fig->rotcnt;
    validfig = _field_validatefig(f);
    if (validfig == Border) {
      int figsiz = f->ff.fig->rots[0].cols;
      bool isvalid = false;
      for (int i = 1; !isvalid && i < figsiz; i = i > 0 ? -i : -i + 1) {
        isvalid = field_movefig(f, f->ff.row, f->ff.col + i);
      }
      if (isvalid) validfig = Ok;
    }
  } while (validfig != Ok);

  return f->ff.rotidx != prev_rotidx;
}

bool field_movefig(field_t *f, int row, int col) {
  int prev_row = f->ff.row, prev_col = f->ff.col;
  f->ff.row = row;
  f->ff.col = col;

  bool validfig = (_field_validatefig(f) == Ok);
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
  blockmatrix_t bm = figure_get(f->ff.fig, f->ff.rotidx);
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (blockmatrix_get(&bm, i, j))
        blockmatrix_set(&f->bm, f->ff.row + i, f->ff.col + j,
                        f->ff.fig->figtype);
    }
  }
}

void field_removefig(field_t *f) {
  // Nothing to be freed: falling figure does not *own* anything
}

static void _field_dropline(field_t *f, size_t droprow) {
  for (size_t row = droprow; row > 0; row--) {
    for (size_t col = 0; col < f->bm.cols; col++) {
      blockmatrix_set(&f->bm, row, col, blockmatrix_get(&f->bm, row - 1, col));
    }
  }
}

int field_droplines(field_t *f) {
  int dropcount = 0;
  for (size_t row = 0; row < f->bm.rows; row++) {
    bool linefilled = true;
    for (size_t col = 0;
         col < f->bm.cols && (linefilled = blockmatrix_get(&f->bm, row, col));
         col++);
    if (linefilled) {
      _field_dropline(f, row);
      dropcount++;
    }
  }
  return dropcount;
}
