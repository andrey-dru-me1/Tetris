#include "brick_game/tetris/tetris.h"

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "brick_game/tetris/defs.h"
#include "brick_game/tetris/falling_figure.h"
#include "brick_game/tetris/field.h"
#include "brick_game/tetris/figset.h"
#include "brick_game/tetris/figure.h"
#include "brick_game/tetris/fsm.h"
#include "brick_game/tetris/game.h"

static game_t game;

void userInput(UserAction_t action, bool hold) {
  committransition(&game, action);
}

static bool checkconstraints(int row, int col, int width, int height) {
  return row >= 0 && row < width && col >= 0 && col < height;
}

GameInfo_t updateCurrentState() {
  struct timeval now;
  gettimeofday(&now, NULL);
  if (now.tv_sec > game.nexttm.tv_sec && now.tv_usec > game.nexttm.tv_usec &&
      game.state == StateRun) {
    committransition(&game, Down);
    game.nexttm = (struct timeval){
        .tv_sec = game.nexttm.tv_sec +
                  ((game.nexttm.tv_usec + SHIFT_INTERVAL) / (long)1e6),
        .tv_usec = (game.nexttm.tv_usec + SHIFT_INTERVAL) % (long)1e6};
  }

  int **intfield = game.info.field;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      intfield[i][j] = bitmatrix_get(&game.field.bm, i, j);
    }
  }

  bitmatrix_t bm = figure_get(game.field.ff.fig, game.field.ff.rotidx);
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (checkconstraints(game.field.ff.row + i, game.field.ff.col + j,
                           game.field.bm.rows, game.field.bm.cols))
        intfield[game.field.ff.row + i][game.field.ff.col + j] |=
            bitmatrix_get(&bm, i, j);
    }
  }

  int prevrow = game.field.ff.row;
  while (field_shiftfig(&game.field));
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (checkconstraints(game.field.ff.row + i, game.field.ff.col + j,
                           game.field.bm.rows, game.field.bm.cols))
        intfield[game.field.ff.row + i][game.field.ff.col + j] |=
            bitmatrix_get(&bm, i, j) * 2;
    }
  }
  game.field.ff.row = prevrow;

  return game.info;
}
