#include <stdio.h>

#include "brick_game/tetris/tetris.h"
#include <stdlib.h>

int main(void) {
  int game = 1;
  userInput(Start, false);
  while (game) {
    char c;
    scanf("%c", &c);
    switch (c) {
      case 'q':
        userInput(Terminate, false);
        game = 0;
        break;
      case 'a':
        userInput(Left, false);
        break;
      case 'd':
        userInput(Right, false);
        break;
      case 's':
        userInput(Down, false);
        break;
      case 'w':
        userInput(Up, false);
        break;
    }

    GameInfo_t gameInfo = updateCurrentState();

    // printf("\e[1;1H\e[2J");

    printf("+");
    for (int i = 0; i < 10 * 2; i++) printf("-");
    printf("+\n");

    for (int i = 0; i < 12; i++) {
      for (int j = 0; j < 10; j++) {
        if(gameInfo.field[i][j]) printf("OO");
        else printf("  ");
      }
      puts("");
    }

    printf("+");
    for (int i = 0; i < 10 * 2; i++) printf("-");
    printf("+\n");

    for (int i = 0; i < 12; i++) free(gameInfo.field[i]);
    free(gameInfo.field);
  }
  return 0;
}