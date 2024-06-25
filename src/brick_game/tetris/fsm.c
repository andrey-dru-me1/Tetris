#include "brick_game/tetris/fsm.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "brick_game/tetris/defs.h"
#include "brick_game/tetris/field.h"
#include "brick_game/tetris/figset.h"

#define MIN(A, B) (A) > (B) ? (B) : (A)

typedef gamestate_t (*transition_fn)(game_t *);

static bool _checkconstraints(int row, int col, int width, int height) {
  return row >= 0 && row < width && col >= 0 && col < height;
}

static gamestate_t _prevstate(gamestate_t *gs) {
  static gamestate_t prev;
  if (gs) prev = *gs;
  return prev;
}

static bool _shiftfig(game_t *game) {
  static int scoring[] = {0, 100, 300, 700, 1500};

  bool validshift = field_shiftfig(&game->field);
  if (!validshift) {
    field_commitfig(&game->field);
    field_removefig(&game->field);
    int linesdropped = field_droplines(&game->field);
    game->info.score += scoring[linesdropped];
  }
  return validshift;
}

static void _addtimeinterval(struct timeval *t, long usec) {
  *t = (struct timeval){.tv_sec = t->tv_sec + ((t->tv_usec + usec) / (long)1e6),
                        .tv_usec = (t->tv_usec + usec) % (long)1e6};
}

static gamestate_t pass(game_t *game) { return game->state; }

static gamestate_t restartgame(game_t *game) {
  game_delete(game);
  game_init(game);
  return StateLaunchFig;
}

static gamestate_t launchfig(game_t *game) {
  static long tickintervals[] = {1e6, 9e5, 8e5, 7e5, 6e5,
                                 5e5, 4e5, 3e5, 2e5, 1e5};

  gamestate_t retstate = StateRun;
  if (!field_spawnfig(&game->field, game->nextfig, -game->nextfig->rowsempty, 3,
                      0)) {
    retstate = StateFailure;
  }
  game->nextfig = game->figset.figs + (rand() % 7);
  game->usectickinterval = tickintervals[MIN(9, game->info.score / 600)];
  _addtimeinterval(&game->nexttm, game->usectickinterval);
  return retstate;
}

static gamestate_t shiftfig(game_t *game) {
  gamestate_t retstate = StateRun;
  if (!_shiftfig(game)) {
    retstate = StateLaunchFig;
  }
  return retstate;
}

static gamestate_t movefigdown(game_t *game) {
  while (_shiftfig(game));
  return StateLaunchFig;
}

static gamestate_t moveright(game_t *game) {
  field_movefig(&game->field, game->field.ff.row, game->field.ff.col + 1);
  return game->state;
}

static gamestate_t moveleft(game_t *game) {
  field_movefig(&game->field, game->field.ff.row, game->field.ff.col - 1);
  return game->state;
}

static gamestate_t rotatefig(game_t *game) {
  field_rotatefig(&game->field);
  return game->state;
}

static gamestate_t setpause(game_t *game) {
  game->info.pause = 1;
  _prevstate(&game->state);
  return StatePause;
}

static gamestate_t resetpause(game_t *game) {
  game->info.pause = 0;
  gettimeofday(&game->nexttm, NULL);
  return _prevstate(NULL);
}

static gamestate_t initgame(game_t *game) {
  game_init(game);
  return StateLaunchFig;
}

static gamestate_t endgame(game_t *game) {
  game_delete(game);
  return StateTerminate;
}

static gamestate_t tryagain(game_t *game) {
  game_delete(game);
  game_init(game);
  return StateLaunchFig;
}

static gamestate_t tick(game_t *game) {
  gamestate_t retstate = game->state;
  struct timeval now;
  gettimeofday(&now, NULL);
  if (now.tv_sec >= game->nexttm.tv_sec &&
      now.tv_usec >= game->nexttm.tv_usec && game->state == StateRun) {
    retstate = shiftfig(game);
    _addtimeinterval(&game->nexttm, game->usectickinterval);
  }
  return retstate;
}

static gamestate_t fillinfofield(game_t *game) {
  int **intfield = game->info.field;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      intfield[i][j] = blockmatrix_get(&game->field.bm, i, j);
    }
  }

  for (int i = 0; i < 4; i++) memset(game->info.next[i], 0, 4 * sizeof(int));

  for (int i = 0; i < game->nextfig->rots[0].rows; i++) {
    for (int j = 0; j < game->nextfig->rots[0].cols; j++) {
      game->info.next[i][j] = blockmatrix_get(&game->nextfig->rots[0], i, j) *
                              game->nextfig->figtype;
    }
  }
  return game->state;
}

static gamestate_t mapfallingfigure(game_t *game) {
  int **intfield = game->info.field;
  blockmatrix_t bm = figure_get(game->field.ff.fig, game->field.ff.rotidx);
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (_checkconstraints(game->field.ff.row + i, game->field.ff.col + j,
                            game->field.bm.rows, game->field.bm.cols) &&
          blockmatrix_get(&bm, i, j))
        intfield[game->field.ff.row + i][game->field.ff.col + j] =
            game->field.ff.fig->figtype;
    }
  }

  int prevrow = game->field.ff.row;
  while (field_shiftfig(&game->field));
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (_checkconstraints(game->field.ff.row + i, game->field.ff.col + j,
                            game->field.bm.rows, game->field.bm.cols) &&
          intfield[game->field.ff.row + i][game->field.ff.col + j] == 0 &&
          blockmatrix_get(&bm, i, j) != 0)
        intfield[game->field.ff.row + i][game->field.ff.col + j] = -1;
    }
  }
  game->field.ff.row = prevrow;
  return game->state;
}

void committransition(game_t *game, gameact_t act) {
  static transition_fn transitions[7][11] = {
      {restartgame, setpause, endgame, moveleft, moveright, movefigdown,
       shiftfig, rotatefig, tick, fillinfofield, mapfallingfigure},
      {restartgame, resetpause, endgame, pass, pass, pass, pass, pass, pass,
       fillinfofield, mapfallingfigure},
      {launchfig, setpause, launchfig, endgame, launchfig, launchfig, launchfig,
       launchfig, launchfig, fillinfofield, launchfig},
      {pass, pass, pass, pass, pass, pass, pass, pass, pass, fillinfofield,
       pass},
      {tryagain, pass, endgame, pass, pass, pass, pass, pass, pass,
       fillinfofield, pass},
      {initgame, initgame, initgame, initgame, initgame, initgame, initgame,
       initgame, initgame, initgame, initgame}};

  game->state = transitions[game->state][act](game);
}
