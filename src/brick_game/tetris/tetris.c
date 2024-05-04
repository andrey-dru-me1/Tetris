#include "brick_game/tetris/tetris.h"

#include <stdlib.h>
#include <time.h>

#include "brick_game/tetris/falling_figure.h"
#include "brick_game/tetris/field.h"
#include "brick_game/tetris/figgen.h"
#include "brick_game/tetris/figure.h"

static void generate_figure_set(figure_t *figset) {
  figgen_t figgens[] = {
      figgen.generate_I_figure, figgen.generate_L_figure,
      figgen.generate_J_figure, figgen.generate_O_figure,
      figgen.generate_T_figure, figgen.generate_S_figure,
      figgen.generate_Z_figure,
  };
  for (int i = 0; i < 7; i++) {
    figset[i] = figgens[i]();
  }
}

field_t field;
figure_t figset[7];
falling_figure_t ff;

static void launchfig() {
  falling_figure.create(&ff, figset + (rand() % 7), &field, 0, 3, 0);
}

static void shiftfig() {
  if (!falling_figure.shift(&ff)) {
    falling_figure.commit(&ff);
    falling_figure.remove(&ff);
    launchfig();
  }
}

static void moveright() { falling_figure.move(&ff, ff.row, ff.col + 1); }

static void moveleft() { falling_figure.move(&ff, ff.row, ff.col - 1); }

static void rotatefig() { falling_figure.rotate(&ff); }

static void initgame() {
  bitmatrix.create(&field, 12, 10);
  generate_figure_set(figset);
  srand(time(NULL));
  launchfig();
}

static void endgame() {
  bitmatrix.remove(&field);
  falling_figure.remove(&ff);
  for (size_t i = 0; i < 7; i++) {
    figure.remove(figset + i);
  }
}

void userInput(UserAction_t action, bool hold) {
  switch (action) {
    case Start:
      initgame();
      break;
    case Right:
      moveright();
      break;
    case Left:
      moveleft();
      break;
    case Down:
      shiftfig();
      break;
    case Up:
      rotatefig();
      break;
    case Terminate:
      endgame();
      break;
    default:
      break;
  }
}

#include <stdlib.h>

static bool checkconstraints(int row, int col, int width, int height) {
  return row >= 0 && row < width && col >= 0 && col < height;
}

GameInfo_t updateCurrentState() {
  int **intfield = calloc(12, sizeof(int *));
  for (int i = 0; i < 12; i++) {
    intfield[i] = calloc(10, sizeof(int));
    for (int j = 0; j < 10; j++) {
      intfield[i][j] = bitmatrix.get(&field, i, j);
    }
  }
  bitmatrix_t bm = figure.get(ff.fig, ff.rotidx);
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (checkconstraints(ff.row + i, ff.col + j, ff.field->rows,
                           ff.field->cols))
        intfield[ff.row + i][ff.col + j] |= bitmatrix.get(&bm, i, j);
    }
  }
  return (GameInfo_t){.field = intfield};
}
