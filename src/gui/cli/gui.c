#include <curses.h>
#include <locale.h>
#include <unistd.h>

#include "brick_game/tetris/tetris.h"

#define WIDTH 10
#define HEIGHT 20

#define SET_SQUARE bkgdset(COLOR_PAIR(1))
#define SET_SPACE bkgdset(COLOR_PAIR(2))
#define RESET_COLOR bkgdset(COLOR_PAIR(0))

static void init_curses() {
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
}

static void print_field(GameInfo_t game_info) {
  move(0, 0);

  addstr("+");
  for (int i = 0; i < WIDTH * 2 - 2; i++) addstr("-");
  addstr("+\n");

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      bool square = false;
      if (game_info.field[i][j] & 1u) {
        SET_SQUARE;
        square = true;
      } else {
        SET_SPACE;
      }
      if ((game_info.field[i][j] & 0b10u) && !square) {
        addch(ACS_BOARD);
        addch(ACS_BOARD);
      } else
        addstr("  ");
    }
    RESET_COLOR;
    addstr("\n\r");
  }

  addstr("+");
  for (int i = 0; i < WIDTH * 2 - 2; i++) addstr("-");
  addstr("+\n\r");
}

static int handle_user_input() {
  int game = 1;

  int c = getch();
  if (c != ERR) {
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
  return game;
}

int main(void) {
  init_curses();

  int game = 1;
  userInput(Start, false);
  while (game) {
    GameInfo_t game_info = updateCurrentState();
    print_field(game_info);
    usleep(10000);  // 1/100 sec
    game = handle_user_input();
  }

  endwin();
  return 0;
}