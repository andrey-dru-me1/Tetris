#include <curses.h>
#include <locale.h>
#include <unistd.h>

#include "brick_game/tetris/tetris.h"

#define WIDTH 10
#define HEIGHT 20

#define SET_SQUARE bkgdset(COLOR_PAIR(1))
#define SET_SPACE bkgdset(COLOR_PAIR(2))
#define RESET_COLOR bkgdset(COLOR_PAIR(0))

int main(void) {
  setlocale(LC_ALL, "");
  WINDOW *stdscr = initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  start_color();

  init_pair(1, COLOR_WHITE, COLOR_WHITE);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);

  int game = 1;
  userInput(Start, false);
  while (game) {
    GameInfo_t gameInfo = updateCurrentState();

    move(0, 0);

    addstr("+");
    for (int i = 0; i < WIDTH * 2-2; i++) addstr("-");
    addstr("+\n");

    for (int i = 0; i < HEIGHT; i++) {
      for (int j = 0; j < WIDTH; j++) {
        bool square = false;
        if (gameInfo.field[i][j] & 1u) {
          SET_SQUARE;
          square = true;
        }
        else {
          SET_SPACE;
        }
        if ((gameInfo.field[i][j] & 0b10u) && !square) {
          addch(ACS_BOARD);
          addch(ACS_BOARD);
        } else
          addstr("  ");
      }
      RESET_COLOR;
      addstr("\n\r");
    }

    usleep(10000);  // 1/100 sec

    addstr("+");
    for (int i = 0; i < WIDTH * 2-2; i++) addstr("-");
    addstr("+\n\r");
    // refresh();

    int c = getch();
    if (c == ERR) continue;
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