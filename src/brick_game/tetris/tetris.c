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

static game_t game = {.state = StateInit};

void userInput(UserAction_t action, bool hold) {
  committransition(&game, action);
}

GameInfo_t updateCurrentState() {
  committransition(&game, ActTick);
  committransition(&game, ActFillInfoField);
  committransition(&game, ActMapFallingFigure);
  return game.info;
}
