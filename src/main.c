#include <stdio.h>
#include <stdlib.h>

#include "brick_game/tetris/tetris.h"

#define WIDTH 10
#define HEIGHT 20

int main(void) {
  int game = 1;
  userInput(Start, false);
  while (game) {
    GameInfo_t gameInfo = updateCurrentState();

    // printf("\e[1;1H\e[2J");

    printf("+");
    for (int i = 0; i < WIDTH * 2; i++) printf("-");
    printf("+\n");

    for (int i = 0; i < HEIGHT; i++) {
      for (int j = 0; j < WIDTH; j++) {
        if (gameInfo.field[i][j])
          printf("OO");
        else
          printf("  ");
      }
      puts("");
    }

    printf("+");
    for (int i = 0; i < WIDTH * 2; i++) printf("-");
    printf("+\n");

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
  }
  return 0;
}