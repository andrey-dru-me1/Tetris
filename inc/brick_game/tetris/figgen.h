#ifndef FIGGEN_H_34987486
#define FIGGEN_H_34987486

#include "brick_game/tetris/figure.h"

typedef figure_t (*figgen_t)();

figure_t figgen_generate_I_figure();
figure_t figgen_generate_L_figure();
figure_t figgen_generate_J_figure();
figure_t figgen_generate_T_figure();
figure_t figgen_generate_O_figure();
figure_t figgen_generate_S_figure();
figure_t figgen_generate_Z_figure();

#endif
