#ifndef GAME_H_6841623
#define GAME_H_6841623

#include <sys/time.h>

#include "brick_game/tetris/field.h"
#include "brick_game/tetris/figset.h"
#include "brick_game/tetris/tetris.h"

typedef enum {
  StateRun,
  StatePause,
  StateLaunchFig,
  StateTerminate,
  StateFailure,
  StateInit
} gamestate_t;

typedef enum {
  ActStart,
  ActPause,
  ActTerminate,
  ActLeft,
  ActRight,
  ActUp,
  ActDown,
  ActAction,
  ActTick,
  ActFillInfoField,
  ActMapFallingFigure
} gameact_t;

typedef struct {
  GameInfo_t info;
  gamestate_t state;
  field_t field;
  figset_t figset;
  figure_t *nextfig;
  struct timeval nexttm;
  long usectickinterval;
} game_t;

void game_init(game_t *game);
void game_delete(game_t *game);

#endif
