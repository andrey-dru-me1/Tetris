#include "brick_game/tetris/figgen.h"

static figure_t generate_I_figure() {
  figure_t figI;
  figure.create(&figI, 2);

  bitmatrix_t bm1, bm2;
  bitmatrix.create(&bm1, 4, 4);
  for (size_t i = 0; i < 4; i++) bitmatrix.set(&bm1, i, 2);
  bm2 = bitmatrix.rotate(&bm1);
  figure.set(&figI, 0, bm1);
  figure.set(&figI, 1, bm2);

  return figI;
}

static figure_t generate_L_figure() {
  figure_t figL;
  figure.create(&figL, 4);

  bitmatrix_t bms[4];
  bitmatrix.create(bms + 0, 3, 3);
  for (size_t i = 0; i < 3; i++) bitmatrix.set(bms + 0, i, 1);
  bitmatrix.set(bms + 0, 2, 2);

  figure.set(&figL, 0, bms[0]);
  for (size_t i = 1; i < 4; i++) {
    bms[i] = bitmatrix.rotate(bms + i - 1);
    figure.set(&figL, i, bms[i]);
  }

  return figL;
}

static figure_t generate_J_figure() {
  figure_t figJ = generate_L_figure();
  for (size_t i = 0; i < figJ.rotcnt; i++) {
    bitmatrix_t rot = figure.get(&figJ, i);
    figure.set(&figJ, i, bitmatrix.flip_vertically(&rot));
    bitmatrix.remove(&rot);
  }
  return figJ;
}

static figure_t generate_T_figure() {
  figure_t figT;
  figure.create(&figT, 4);

  bitmatrix_t bms[4];
  bitmatrix.create(bms + 0, 3, 3);
  for (size_t i = 0; i < 3; i++) {
    bitmatrix.set(bms + 0, 1, i);
  }
  bitmatrix.set(bms + 0, 0, 1);

  figure.set(&figT, 0, bms[0]);
  for (size_t i = 1; i < 4; i++) {
    bms[i] = bitmatrix.rotate(bms + i - 1);
    figure.set(&figT, i, bms[i]);
  }
  return figT;
}

static figure_t generate_O_figure() {
  figure_t figO;
  figure.create(&figO, 1);

  bitmatrix_t bm;
  bitmatrix.create(&bm, 2, 2);
  for (size_t i = 0; i < 2; i++) {
    for (size_t j = 0; j < 2; j++) {
      bitmatrix.set(&bm, i, j);
    }
  }
  figure.set(&figO, 0, bm);
  return figO;
}

static figure_t generate_S_figure() {
  figure_t figS;
  figure.create(&figS, 2);

  bitmatrix_t bms[2];
  bitmatrix.create(bms + 0, 3, 3);
  bitmatrix.set(bms + 0, 1, 0);
  bitmatrix.set(bms + 0, 1, 1);
  bitmatrix.set(bms + 0, 2, 1);
  bitmatrix.set(bms + 0, 2, 2);
  bms[1] = bitmatrix.rotate(bms + 0);

  figure.set(&figS, 0, bms[0]);
  figure.set(&figS, 1, bms[1]);

  return figS;
}

static figure_t generate_Z_figure() {
  figure_t figZ = generate_S_figure();
  for (size_t i = 0; i < figZ.rotcnt; i++) {
    bitmatrix_t rot = figure.get(&figZ, i);
    figure.set(&figZ, i, bitmatrix.flip_vertically(&rot));
    bitmatrix.remove(&rot);
  }
  return figZ;
}

struct figgen_module figgen = {
  .generate_I_figure = generate_I_figure,
  .generate_L_figure = generate_L_figure,
  .generate_J_figure = generate_J_figure,
  .generate_O_figure = generate_O_figure,
  .generate_T_figure = generate_T_figure,
  .generate_S_figure = generate_S_figure,
  .generate_Z_figure = generate_Z_figure
};
