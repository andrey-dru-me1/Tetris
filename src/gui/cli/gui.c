#include <curses.h>
#include <locale.h>

#include "brick_game/tetris/tetris.h"

#define WIDTH 10
#define HEIGHT 20

int main(void) {
  setlocale(LC_ALL, "");
  WINDOW *stdscr = initscr();
  cbreak();
  noecho();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  start_color();

  printf("lol");

  int game = 1;
  userInput(Start, false);
  while (game) {
    GameInfo_t gameInfo = updateCurrentState();

    move(0, 0);

    addstr("+");
    for (int i = 0; i < WIDTH * 2; i++) addstr("-");
    addstr("+\n");

    for (int i = 0; i < HEIGHT; i++) {
      for (int j = 0; j < WIDTH; j++) {
        if (gameInfo.field[i][j])
          addstr("OO");
        else
          addstr("  ");
      }
      addstr("\n\r");
    }

    addstr("+");
    for (int i = 0; i < WIDTH * 2; i++) addstr("-");
    addstr("+\n\r");
    // refresh();

    int c = getch();
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

  endwin();
  return 0;
}