#ifndef FIELD_H_68165
#define FIELD_H_68165

#include "brick_game/tetris/falling_figure.h"
#include "brick_game/tetris/types/blockmatrix.h"

typedef struct {
  blockmatrix_t bm;
  falling_figure_t ff;
} field_t;

bool field_spawnfig(field_t *f, figure_t *fig, int row, int col, int rotidx);
bool field_rotatefig(field_t *f);
bool field_shiftfig(field_t *f);
bool field_movefig(field_t *f, int row, int col);
void field_commitfig(field_t *f);
void field_removefig(field_t *f);
int field_droplines(field_t *f);

#endif
