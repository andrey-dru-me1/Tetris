#include "brick_game/tetris/tetris.h"

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "brick_game/tetris/falling_figure.h"
#include "brick_game/tetris/field.h"
#include "brick_game/tetris/figset.h"
#include "brick_game/tetris/figure.h"

#define WIDTH 10
#define HEIGHT 20

field_t field;
falling_figure_t ff;
struct timeval next;

#define SHIFT_INTERVAL 500000

static void dropline(size_t droprow) {
  for (size_t row = droprow; row > 0; row--) {
    for (size_t col = 0; col < field.cols; col++) {
      bitmatrix_set_bit(&field, row, col, bitmatrix_get(&field, row - 1, col));
    }
  }
}

static int droplines() {
  int dropcount = 0;
  for (size_t row = 0; row < field.rows; row++) {
    bool linefilled = true;
    for (size_t col = 0;
         col < field.cols && (linefilled = bitmatrix_get(&field, row, col));
         col++);
    if (linefilled) {
      dropline(row);
      dropcount++;
    }
  }
  return dropcount;
}

static void launchfig() {
  falling_figure_create(&ff, figset() + (rand() % 7), &field, 0, 3, 0);
}

static void shiftfig() {
  if (!falling_figure_shift(&ff)) {
    falling_figure_commit(&ff);
    falling_figure_remove(&ff);
    droplines();
    launchfig();
  }
}

static void moveright() { falling_figure_move(&ff, ff.row, ff.col + 1); }

static void moveleft() { falling_figure_move(&ff, ff.row, ff.col - 1); }

static void rotatefig() { falling_figure_rotate(&ff); }

static void initgame() {
  bitmatrix_create(&field, HEIGHT, WIDTH);
  srand(time(NULL));
  launchfig();

  gettimeofday(&next, NULL);
}

static void endgame() {
  bitmatrix_remove(&field);
  falling_figure_remove(&ff);
  for (size_t i = 0; i < 7; i++) {
    figure_remove(figset() + i);
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
      intfield[i][j] = bitmatrix_get(&field, i, j);
    }
  }

  bitmatrix_t bm = figure_get(ff.fig, ff.rotidx);
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (checkconstraints(ff.row + i, ff.col + j, ff.field->rows,
                           ff.field->cols))
        intfield[ff.row + i][ff.col + j] |= bitmatrix_get(&bm, i, j);
    }
  }

  int prevrow = ff.row;
  while (falling_figure_shift(&ff));
  for (int i = 0; i < bm.rows; i++) {
    for (int j = 0; j < bm.cols; j++) {
      if (checkconstraints(ff.row + i, ff.col + j, ff.field->rows,
                           ff.field->cols))
        intfield[ff.row + i][ff.col + j] |= bitmatrix_get(&bm, i, j) * 2;
    }
  }
  ff.row = prevrow;

  return *game_info;
}
