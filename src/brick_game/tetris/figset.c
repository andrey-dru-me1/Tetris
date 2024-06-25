#include "brick_game/tetris/figset.h"

#include <stdbool.h>

static figure_t figgen_generate_I_figure() {
  figure_t fig;
  figure_createfromarray(&fig, 4, 4, 2, I,
                         (block_t[]){
                             0, 0, 0, 0,  // 0
                             0, 0, 0, 0,  // 1
                             1, 1, 1, 1,  // 2
                             0, 0, 0, 0   // 3
                         });
  return fig;
}

static figure_t figgen_generate_L_figure() {
  figure_t fig;
  figure_createfromarray(&fig, 3, 3, 4, L,
                         (block_t[]){
                             0, 0, 0,  // 0
                             1, 1, 1,  // 1
                             1, 0, 0,  // 2
                         });
  return fig;
}

static figure_t figgen_generate_J_figure() {
  figure_t fig;
  figure_createfromarray(&fig, 3, 3, 4, J,
                         (block_t[]){
                             0, 0, 0,  // 0
                             1, 1, 1,  // 1
                             0, 0, 1,  // 2
                         });
  return fig;
}

static figure_t figgen_generate_T_figure() {
  figure_t fig;
  figure_createfromarray(&fig, 3, 3, 4, T,
                         (block_t[]){
                             0, 0, 0,  // 0
                             1, 1, 1,  // 1
                             0, 1, 0,  // 2
                         });
  return fig;
}

static figure_t figgen_generate_O_figure() {
  figure_t fig;
  figure_createfromarray(&fig, 2, 2, 1, O,
                         (block_t[]){
                             1, 1,  // 0
                             1, 1,  // 1
                         });
  return fig;
}

static figure_t figgen_generate_S_figure() {
  figure_t fig;
  figure_createfromarray(&fig, 3, 3, 2, S,
                         (block_t[]){
                             0, 0, 0,  // 0
                             0, 1, 1,  // 1
                             1, 1, 0,  // 2
                         });
  return fig;
}

static figure_t figgen_generate_Z_figure() {
  figure_t fig;
  figure_createfromarray(&fig, 3, 3, 2, Z,
                         (block_t[]){
                             0, 0, 0,  // 0
                             1, 1, 0,  // 1
                             0, 1, 1,  // 2
                         });
  return fig;
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
