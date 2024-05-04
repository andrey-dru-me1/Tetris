#ifndef FIGGEN_H_34987486
#define FIGGEN_H_34987486

#include "brick_game/tetris/figure.h"

typedef figure_t (*figgen_t)();

struct figgen_module {
  figgen_t generate_I_figure;
  figgen_t generate_L_figure;
  figgen_t generate_J_figure;
  figgen_t generate_T_figure;
  figgen_t generate_O_figure;
  figgen_t generate_S_figure;
  figgen_t generate_Z_figure;
};

extern struct figgen_module figgen;

#endif
