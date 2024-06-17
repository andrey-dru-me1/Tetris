#include "brick_game/tetris/figset.h"

#include <stdbool.h>

static figure_t figgen_generate_I_figure() {
  figure_t figI;
  figure_create(&figI, 2);

  bitmatrix_t bm1, bm2;
  bitmatrix_create(&bm1, 4, 4);
  for (size_t i = 0; i < 4; i++) bitmatrix_set(&bm1, i, 2, 1);
  bm2 = bitmatrix_rotate(&bm1);
  figure_set(&figI, 0, bm1);
  figure_set(&figI, 1, bm2);

  return figI;
}

static figure_t figgen_generate_L_figure() {
  figure_t figL;
  figure_create(&figL, 4);

  bitmatrix_t bms[4];
  bitmatrix_create(bms + 0, 3, 3);
  for (size_t i = 0; i < 3; i++) bitmatrix_set(bms + 0, i, 1, 1);
  bitmatrix_set(bms + 0, 2, 2, 1);

  figure_set(&figL, 0, bms[0]);
  for (size_t i = 1; i < 4; i++) {
    bms[i] = bitmatrix_rotate(bms + i - 1);
    figure_set(&figL, i, bms[i]);
  }

  return figL;
}

static figure_t figgen_generate_J_figure() {
  figure_t figJ = figgen_generate_L_figure();
  for (size_t i = 0; i < figJ.rotcnt; i++) {
    bitmatrix_t rot = figure_get(&figJ, i);
    figure_set(&figJ, i, bitmatrix_flip_vertically(&rot));
    bitmatrix_remove(&rot);
  }
  return figJ;
}

static figure_t figgen_generate_T_figure() {
  figure_t figT;
  figure_create(&figT, 4);

  bitmatrix_t bms[4];
  bitmatrix_create(bms + 0, 3, 3);
  for (size_t i = 0; i < 3; i++) {
    bitmatrix_set(bms + 0, 1, i, 1);
  }
  bitmatrix_set(bms + 0, 0, 1, 1);

  figure_set(&figT, 0, bms[0]);
  for (size_t i = 1; i < 4; i++) {
    bms[i] = bitmatrix_rotate(bms + i - 1);
    figure_set(&figT, i, bms[i]);
  }
  return figT;
}

static figure_t figgen_generate_O_figure() {
  figure_t figO;
  figure_create(&figO, 1);

  bitmatrix_t bm;
  bitmatrix_create(&bm, 2, 2);
  for (size_t i = 0; i < 2; i++) {
    for (size_t j = 0; j < 2; j++) {
      bitmatrix_set(&bm, i, j, 1);
    }
  }
  figure_set(&figO, 0, bm);
  return figO;
}

static figure_t figgen_generate_S_figure() {
  figure_t figS;
  figure_create(&figS, 2);

  bitmatrix_t bms[2];
  bitmatrix_create(bms + 0, 3, 3);
  bitmatrix_set(bms + 0, 1, 0, 1);
  bitmatrix_set(bms + 0, 1, 1, 1);
  bitmatrix_set(bms + 0, 2, 1, 1);
  bitmatrix_set(bms + 0, 2, 2, 1);
  bms[1] = bitmatrix_rotate(bms + 0);

  figure_set(&figS, 0, bms[0]);
  figure_set(&figS, 1, bms[1]);

  return figS;
}

static figure_t figgen_generate_Z_figure() {
  figure_t figZ = figgen_generate_S_figure();
  for (size_t i = 0; i < figZ.rotcnt; i++) {
    bitmatrix_t rot = figure_get(&figZ, i);
    figure_set(&figZ, i, bitmatrix_flip_vertically(&rot));
    bitmatrix_remove(&rot);
  }
  return figZ;
}

void figset_init(figset_t *figset) {
  typedef figure_t (*figgen_t)();

  figgen_t figgens[] = {
      figgen_generate_I_figure, figgen_generate_L_figure,
      figgen_generate_J_figure, figgen_generate_O_figure,
      figgen_generate_T_figure, figgen_generate_S_figure,
      figgen_generate_Z_figure,
  };
  for (int i = 0; i < 7; i++) {
    figset->figs[i] = figgens[i]();
  }
}

void figset_free(figset_t *figset) {
  for (int i = 0; i < 7; i++) {
    figure_remove(figset->figs + i);
  }
}
