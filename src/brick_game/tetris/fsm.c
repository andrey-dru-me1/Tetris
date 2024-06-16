#include "brick_game/tetris/fsm.h"

#include <stdlib.h>
#include <time.h>

#include "brick_game/tetris/figset.h"
#include "brick_game/tetris/defs.h"
#include "brick_game/tetris/field.h"

typedef void (*transition_fn)(game_t *);

static void pass(game_t *game);
static void restartgame(game_t *game);
static void droplines(game_t *game);
static void launchfig(game_t *game);
static void shiftfig(game_t *game);
static void movefigdown(game_t *game);
static void moveright(game_t *game);
static void moveleft(game_t *game);
static void rotatefig(game_t *game);
static void setpause(game_t *game);
static void resetpause(game_t *game);
static void initgame(game_t *game);
static void endgame(game_t *game);
static void pass(game_t *game) {}

static void restartgame(game_t *game) {
  endgame(game);
  initgame(game);
}

static void droplines(game_t *game) {
  field_droplines(&game->field);
}

static void launchfig(game_t *game) {
  if (!field_spawnfig(&game->field, figset() + (rand() % 7), 0, 3, 0)) {
    restartgame(game);
  }
}

static void shiftfig(game_t *game) {
  if (!field_shiftfig(&game->field)) {
    field_commitfig(&game->field);
    field_removefig(&game->field);
    field_droplines(&game->field);
    launchfig(game);
  }
}

static void movefigdown(game_t *game) {
  while (field_shiftfig(&game->field));
  field_commitfig(&game->field);
  field_removefig(&game->field);
  field_droplines(&game->field);
  launchfig(game);
}

static void moveright(game_t *game) {
  field_movefig(&game->field, game->field.ff.row, game->field.ff.col + 1);
}

static void moveleft(game_t *game) {
  field_movefig(&game->field, game->field.ff.row, game->field.ff.col - 1);
}

static void rotatefig(game_t *game) { field_rotatefig(&game->field); }

static void setpause(game_t *game) {
  game->state = StatePause;
  game->info.pause = 1;
}

static void resetpause(game_t *game) {
  game->state = StateRun;
  game->info.pause = 0;
  gettimeofday(&game->nexttm, NULL);
}

static void initgame(game_t *game) {
  game->info.field = calloc(HEIGHT, sizeof(*game->info.field));
  int *field = calloc(WIDTH * HEIGHT, sizeof(*field));
  for (size_t row = 0; row < HEIGHT; row++)
    game->info.field[row] = field + row * WIDTH;
  game->info.next = calloc(4, sizeof(*game->info.next));
  int *next = calloc(4 * 4, sizeof(*next));
  for (size_t i = 0; i < 4; i++) game->info.next[i] = next + i * 4;
  game->info.high_score = 0;
  game->info.level = 0;
  game->info.score = 0;
  game->info.speed = 0;

  game->state = StateRun;

  bitmatrix_create(&game->field.bm, HEIGHT, WIDTH);
  srand(time(NULL));
  launchfig(game);
  gettimeofday(&game->nexttm, NULL);
}

static void endgame(game_t *game) {
  free(*game->info.field);
  free(game->info.field);
  game->info.field = NULL;
  free(*game->info.next);
  free(game->info.next);
  game->info.field = NULL;

  bitmatrix_remove(&game->field.bm);
  field_removefig(&game->field);
  figset_free();
}

void committransition(game_t *game, UserAction_t act) {
  static transition_fn transitions[2][8] = {
      {initgame, setpause, endgame, moveleft, moveright, rotatefig, shiftfig,
       movefigdown},
      {restartgame, resetpause, endgame, pass, pass, pass, pass, pass}};

  transitions[game->state][act](game);
}
