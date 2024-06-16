#ifndef GAME_H_6841623
#define GAME_H_6841623

#include <sys/time.h>

#include "brick_game/tetris/tetris.h"
#include "brick_game/tetris/field.h"

typedef enum { StateRun, StatePause } gamestate_e;
typedef struct {
  GameInfo_t info;
  gamestate_e state;
  field_t field;
  struct timeval nexttm;
} game_t;

#endif
