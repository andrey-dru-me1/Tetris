#include "brick_game/tetris/tetris.h"

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "brick_game/tetris/falling_figure.h"
#include "brick_game/tetris/field.h"
#include "brick_game/tetris/figset.h"
#include "brick_game/tetris/figure.h"

typedef enum { StateRun, StatePause } gamestate_e;
typedef struct {
  GameInfo_t info;
  gamestate_e state;
  field_t field;
  struct timeval nexttm;
} game_t;

#define WIDTH 10
#define HEIGHT 20

static void dropline(size_t droprow);
static int droplines();

static void launchfig();
static void shiftfig();
static void moveright();
static void moveleft();
static void rotatefig();
static void togglepause();

static void initgame();
static void endgame();

static bool checkconstraints(int row, int col, int width, int height);

static game_t game;

#define SHIFT_INTERVAL 500000

static void dropline(size_t droprow) {
  for (size_t row = droprow; row > 0; row--) {
    for (size_t col = 0; col < game.field.bm.cols; col++) {
      bitmatrix_set_bit(&game.field.bm, row, col,
                        bitmatrix_get(&game.field.bm, row - 1, col));
    }
  }
}

static int droplines() {
  int dropcount = 0;
  for (size_t row = 0; row < game.field.bm.rows; row++) {
    bool linefilled = true;
    for (size_t col = 0; col < game.field.bm.cols &&
                         (linefilled = bitmatrix_get(&game.field.bm, row, col));
         col++);
    if (linefilled) {
      dropline(row);
      dropcount++;
    }
  }
  return dropcount;
}

static void launchfig() {
  if (!field_spawnfig(&game.field, figset() + (rand() % 7), 0, 3, 0)) {
    endgame();
    initgame();
  }
}

static void shiftfig() {
  if (!field_shiftfig(&game.field)) {
    field_commitfig(&game.field);
    field_removefig(&game.field);
    droplines();
    launchfig();
  }
}

static void movefigdown() {
  while (field_shiftfig(&game.field));
  field_commitfig(&game.field);
  field_removefig(&game.field);
  droplines();
  launchfig();
}

static void moveright() {
  field_movefig(&game.field, game.field.ff.row, game.field.ff.col + 1);
}

static void moveleft() {
  field_movefig(&game.field, game.field.ff.row, game.field.ff.col - 1);
}

static void rotatefig() { field_rotatefig(&game.field); }

static void togglepause() {
  if (game.state == StateRun)
    game.state = StatePause;
  else if (game.state == StatePause)
    game.state = StateRun;
  gettimeofday(&game.nexttm, NULL);
}

static void initgame() {
  game.info.field = calloc(HEIGHT, sizeof(*game.info.field));
  for (size_t row = 0; row < HEIGHT; row++)
    game.info.field[row] = calloc(WIDTH, sizeof(**game.info.field));
  game.info.next = calloc(4, sizeof(*game.info.next));
  for (size_t i = 0; i < 4; i++)
    game.info.next[i] = calloc(4, sizeof(**game.info.next));
  game.info.high_score = 0;
  game.info.level = 0;
  game.info.score = 0;
  game.info.speed = 0;

  bitmatrix_create(&game.field.bm, HEIGHT, WIDTH);
  srand(time(NULL));
  launchfig();
  gettimeofday(&game.nexttm, NULL);
}

static void endgame() {
  for (size_t row = 0; row < HEIGHT; row++)
    free(game.info.field[row]);
  free(game.info.field);
  game.info.field = NULL;
  for (size_t i = 0; i < 4; i++)
    free(game.info.next[i]);
  free(game.info.next);

  bitmatrix_remove(&game.field.bm);
  field_removefig(&game.field);
  figset_free();
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
    case Action:
      movefigdown();
      break;
    case Pause:
      togglepause();
      break;
    default:
      break;
  }
}

static bool checkconstraints(int row, int col, int width, int height) {
  return row >= 0 && row < width && col >= 0 && col < height;
}

GameInfo_t updateCurrentState() {
  struct timeval now;
  gettimeofday(&now, NULL);
  if (now.tv_sec > game.nexttm.tv_sec && now.tv_usec > game.nexttm.tv_usec &&
      game.state == StateRun) {
    shiftfig();
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
