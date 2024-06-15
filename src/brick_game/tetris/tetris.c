#include "brick_game/tetris/tetris.h"

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>

#include "brick_game/tetris/falling_figure.h"
#include "brick_game/tetris/field.h"
#include "brick_game/tetris/figset.h"
#include "brick_game/tetris/figure.h"

#define WIDTH 10
#define HEIGHT 20

static void dropline(size_t droprow);
static int droplines();

static void launchfig();
static void shiftfig();
static void moveright();
static void moveleft();
static void rotatefig();

static void initgame();
static void endgame();

static bool checkconstraints(int row, int col, int width, int height);
static GameInfo_t *get_game_info();

field_t field;
struct timeval next;

#define SHIFT_INTERVAL 500000

static void dropline(size_t droprow) {
  for (size_t row = droprow; row > 0; row--) {
    for (size_t col = 0; col < field.bm.cols; col++) {
      bitmatrix_set_bit(&field.bm, row, col,
                        bitmatrix_get(&field.bm, row - 1, col));
    }
  }
}

static int droplines() {
  int dropcount = 0;
  for (size_t row = 0; row < field.bm.rows; row++) {
    bool linefilled = true;
    for (size_t col = 0; col < field.bm.cols &&
                         (linefilled = bitmatrix_get(&field.bm, row, col));
         col++);
    if (linefilled) {
      dropline(row);
      dropcount++;
    }
  }
  return dropcount;
}

static void launchfig() {
  if (!field_spawnfig(&field, figset() + (rand() % 7), 0, 3, 0)) {
    endgame();
    initgame();
  }
}

static void shiftfig() {
  if (!field_shiftfig(&field)) {
    field_commitfig(&field);
    field_removefig(&field);
    droplines();
    launchfig();
  }
}

static void moveright() {
  field_movefig(&field, field.ff.row, field.ff.col + 1);
}

static void moveleft() {
  field_movefig(&field, field.ff.row, field.ff.col - 1);
}

static void rotatefig() { field_rotatefig(&field); }

static void initgame() {
  bitmatrix_create(&field.bm, HEIGHT, WIDTH);
  srand(time(NULL));
  launchfig();

  gettimeofday(&next, NULL);
}

static void endgame() {
  bitmatrix_remove(&field.bm);
  field_removefig(&field);
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
    default:
      break;
  }
}

static bool checkconstraints(int row, int col, int width, int height) {
  return row >= 0 && row < width && col >= 0 && col < height;
}

static GameInfo_t *get_game_info() {
  static int _intfield[HEIGHT][WIDTH];
  static int *intfield[HEIGHT];

  static int _nextfig[4][4];
  static int *nextfig[4];

  static GameInfo_t game_info = {.field = intfield, .next = nextfig};

  static bool initialized = false;

  if (!initialized) {
    for (size_t i = 0; i < HEIGHT; i++) intfield[i] = _intfield[i];
    for (size_t i = 0; i < 4; i++) nextfig[i] = _nextfig[i];

    memset(_intfield, 0, WIDTH * HEIGHT * sizeof(**_intfield));
    memset(_nextfig, 0, 4 * 4 * sizeof(**_nextfig));

    initialized = true;
  }

  return &game_info;
}

GameInfo_t updateCurrentState() {
  struct timeval now;
  gettimeofday(&now, NULL);
  if (now.tv_sec > next.tv_sec && now.tv_usec > next.tv_usec) {
    shiftfig();
    next = (struct timeval){
        .tv_sec = next.tv_sec + ((next.tv_usec + SHIFT_INTERVAL) / (long)1e6),
        .tv_usec = (next.tv_usec + SHIFT_INTERVAL) % (long)1e6};
  }

  GameInfo_t *game_info = get_game_info();
  int **intfield = game_info->field;
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      intfield[i][j] = bitmatrix_get(&field.bm, i, j);
    }
  }

  bitmatrix_t bm = figure_get(field.ff.fig, field.ff.rotidx);
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (checkconstraints(field.ff.row + i, field.ff.col + j, field.bm.rows,
                           field.bm.cols))
        intfield[field.ff.row + i][field.ff.col + j] |= bitmatrix_get(&bm, i, j);
    }
  }

  int prevrow = field.ff.row;
  while (field_shiftfig(&field));
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (checkconstraints(field.ff.row + i, field.ff.col + j, field.bm.rows,
                           field.bm.cols))
        intfield[field.ff.row + i][field.ff.col + j] |= bitmatrix_get(&bm, i, j) * 2;
    }
  }
  field.ff.row = prevrow;

  return *game_info;
}
