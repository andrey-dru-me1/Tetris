#include "brick_game/tetris/fsm.h"

#include <stdlib.h>
#include <time.h>

#include "brick_game/tetris/defs.h"
#include "brick_game/tetris/field.h"
#include "brick_game/tetris/figset.h"

typedef gamestate_e (*transition_fn)(game_t *);

static void _droplines(game_t *game) { field_droplines(&game->field); }

static bool _checkconstraints(int row, int col, int width, int height) {
  return row >= 0 && row < width && col >= 0 && col < height;
}

static gamestate_e pass(game_t *game) { return game->state; }

static gamestate_e restartgame(game_t *game) {
  game_delete(game);
  game_init(game);
  return StateIdle;
}

static gamestate_e launchfig(game_t *game) {
  gamestate_e retstate = StateRun;
  if (!field_spawnfig(&game->field, figset() + (rand() % 7), 0, 3, 0)) {
    retstate = StateFailure;
  }
  return retstate;
}

static gamestate_e shiftfig(game_t *game) {
  gamestate_e retstate = StateRun;
  if (!field_shiftfig(&game->field)) {
    field_commitfig(&game->field);
    field_removefig(&game->field);
    field_droplines(&game->field);
    retstate = StateLaunchFig;
  }
  return retstate;
}

static gamestate_e movefigdown(game_t *game) {
  while (field_shiftfig(&game->field));
  field_commitfig(&game->field);
  field_removefig(&game->field);
  field_droplines(&game->field);
  return StateLaunchFig;
}

static gamestate_e moveright(game_t *game) {
  field_movefig(&game->field, game->field.ff.row, game->field.ff.col + 1);
  return game->state;
}

static gamestate_e moveleft(game_t *game) {
  field_movefig(&game->field, game->field.ff.row, game->field.ff.col - 1);
  return game->state;
}

static gamestate_e rotatefig(game_t *game) {
  field_rotatefig(&game->field);
  return game->state;
}

static gamestate_e setpause(game_t *game) {
  game->info.pause = 1;
  return StatePause;
}

static gamestate_e resetpause(game_t *game) {
  game->info.pause = 0;
  gettimeofday(&game->nexttm, NULL);
  return StateRun;
}

static gamestate_e initgame(game_t *game) {
  game_init(game);
  return StateIdle;
}

static gamestate_e endgame(game_t *game) {
  game_delete(game);
  return StateTerminate;
}

static gamestate_e tryagain(game_t *game) {
  game_delete(game);
  game_init(game);
  return StateLaunchFig;
}

static gamestate_e tick(game_t *game) {
  gamestate_e retstate = game->state;
  struct timeval now;
  gettimeofday(&now, NULL);
  if (now.tv_sec > game->nexttm.tv_sec && now.tv_usec > game->nexttm.tv_usec &&
      game->state == StateRun) {
    retstate = StateShiftFig;
    game->nexttm = (struct timeval){
        .tv_sec = game->nexttm.tv_sec +
                  ((game->nexttm.tv_usec + SHIFT_INTERVAL) / (long)1e6),
        .tv_usec = (game->nexttm.tv_usec + SHIFT_INTERVAL) % (long)1e6};
  }
  return retstate;
}

static gamestate_e startgame(game_t *game) {
  gettimeofday(&game->nexttm, NULL);
  return StateLaunchFig;
}

static gamestate_e fillinfofield(game_t *game) {
  int **intfield = game->info.field;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      intfield[i][j] = bitmatrix_get(&game->field.bm, i, j);
    }
  }
  return game->state;
}

static gamestate_e mapfallingfigure(game_t *game) {
  int **intfield = game->info.field;
  bitmatrix_t bm = figure_get(game->field.ff.fig, game->field.ff.rotidx);
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (_checkconstraints(game->field.ff.row + i, game->field.ff.col + j,
                            game->field.bm.rows, game->field.bm.cols))
        intfield[game->field.ff.row + i][game->field.ff.col + j] |=
            bitmatrix_get(&bm, i, j);
    }
  }

  int prevrow = game->field.ff.row;
  while (field_shiftfig(&game->field));
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (_checkconstraints(game->field.ff.row + i, game->field.ff.col + j,
                            game->field.bm.rows, game->field.bm.cols))
        intfield[game->field.ff.row + i][game->field.ff.col + j] |=
            bitmatrix_get(&bm, i, j) * 2;
    }
  }
  game->field.ff.row = prevrow;
  return game->state;
}

void committransition(game_t *game, gameact_e act) {
  static transition_fn transitions[8][11] = {
      {restartgame, setpause, endgame, moveleft, moveright, rotatefig, shiftfig,
       movefigdown, tick, fillinfofield, mapfallingfigure},
      {restartgame, resetpause, endgame, pass, pass, pass, pass, pass, pass,
       fillinfofield, mapfallingfigure},
      {startgame, pass, endgame, pass, pass, pass, pass, pass, pass,
       fillinfofield, pass},
      {launchfig, launchfig, launchfig, launchfig, launchfig, launchfig,
       launchfig, launchfig, launchfig, fillinfofield, mapfallingfigure},
      {pass, pass, pass, pass, pass, pass, pass, pass, pass, fillinfofield,
       pass},
      {tryagain, pass, endgame, pass, pass, pass, pass, pass, pass,
       fillinfofield, mapfallingfigure},
      {shiftfig, shiftfig, shiftfig, shiftfig, shiftfig, shiftfig, shiftfig,
       shiftfig, shiftfig, fillinfofield, mapfallingfigure},
      {initgame, initgame, initgame, initgame, initgame, initgame, initgame,
       initgame, initgame, initgame, initgame}};

  game->state = transitions[game->state][act](game);
}
