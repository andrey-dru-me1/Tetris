#include "brick_game/tetris/figset.h"

#include <stdbool.h>

static figure_t figgen_generate_I_figure() {
  figure_t figI;
  figure_create(&figI, 4, 4, 2);
  for (size_t i = 0; i < 4; i++) blockmatrix_set(figI.rots + 0, i, 2, 1);
  blockmatrix_rotate(figI.rots + 0, figI.rots + 1);
  return figI;
}

static figure_t figgen_generate_L_figure() {
  figure_t figL;
  figure_create(&figL, 3, 3, 4);
  for (size_t i = 0; i < 3; i++) blockmatrix_set(figL.rots + 0, i, 1, 1);
  blockmatrix_set(figL.rots + 0, 2, 2, 1);
  for (size_t i = 1; i < 4; i++)
    blockmatrix_rotate(figL.rots + i - 1, figL.rots + i);
  return figL;
}

static figure_t figgen_generate_J_figure() {
  figure_t figJ = figgen_generate_L_figure();
  for (size_t i = 0; i < figJ.rotcnt; i++)
    blockmatrix_flip_vertically(figJ.rots + i, figJ.rots + i);
  return figJ;
}

static figure_t figgen_generate_T_figure() {
  figure_t figT;
  figure_create(&figT, 3, 3, 4);

  for (size_t i = 0; i < 3; i++) blockmatrix_set(figT.rots + 0, 1, i, 1);
  blockmatrix_set(figT.rots + 0, 0, 1, 1);

  for (size_t i = 1; i < 4; i++)
    blockmatrix_rotate(figT.rots + i - 1, figT.rots + i);
  return figT;
}

static figure_t figgen_generate_O_figure() {
  figure_t figO;
  figure_create(&figO, 2, 2, 1);

  for (size_t i = 0; i < 2; i++)
    for (size_t j = 0; j < 2; j++) blockmatrix_set(figO.rots, i, j, 1);
  return figO;
}

static figure_t figgen_generate_S_figure() {
  figure_t figS;
  figure_create(&figS, 3, 3, 2);

  blockmatrix_set(figS.rots + 0, 1, 0, 1);
  blockmatrix_set(figS.rots + 0, 1, 1, 1);
  blockmatrix_set(figS.rots + 0, 2, 1, 1);
  blockmatrix_set(figS.rots + 0, 2, 2, 1);
  blockmatrix_rotate(figS.rots + 0, figS.rots + 1);

  return figS;
}

static figure_t figgen_generate_Z_figure() {
  figure_t figZ = figgen_generate_S_figure();
  for (size_t i = 0; i < figZ.rotcnt; i++)
    blockmatrix_flip_vertically(figZ.rots + i, figZ.rots + i);
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
