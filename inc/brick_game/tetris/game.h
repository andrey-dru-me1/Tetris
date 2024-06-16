#ifndef GAME_H_6841623
#define GAME_H_6841623

#include <sys/time.h>

#include "brick_game/tetris/field.h"
#include "brick_game/tetris/tetris.h"

typedef enum { StateRun, StatePause } gamestate_e;
typedef struct {
  GameInfo_t info;
  gamestate_e state;
  field_t field;
  struct timeval nexttm;
} game_t;

void game_init(game_t *game);
void game_delete(game_t *game);

#endif
