#include <curses.h>
#include <locale.h>
#include <unistd.h>

#include "brick_game/tetris/tetris.h"

#define WIDTH 10
#define HEIGHT 20

#define SET_SQUARE bkgdset(COLOR_PAIR(1))
#define SET_SPACE bkgdset(COLOR_PAIR(2))
#define SET_BORDER bkgdset(COLOR_PAIR(3))
#define RESET_COLOR bkgdset(COLOR_PAIR(0))

static void init_curses() {
  setlocale(LC_ALL, "");
  WINDOW *stdscr = initscr();
  cbreak();
  curs_set(0);
  noecho();
  nodelay(stdscr, TRUE);
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  start_color();

  init_pair(1, COLOR_WHITE, COLOR_WHITE);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_WHITE, COLOR_GREEN);
}

static void print_border() {
  SET_BORDER;
  move(0, 0);
  for (int i = 0; i < WIDTH + 2; i++) addstr("  ");
  move(HEIGHT + 1, 0);
  for (int i = 0; i < WIDTH + 2; i++) addstr("  ");

  for (int i = 0; i < HEIGHT + 1; i++) {
    mvaddstr(i + 1, 0, "  ");
    mvaddstr(i + 1, WIDTH * 2 + 2, "  ");
  }
}

static void print_field(GameInfo_t game_info) {
  for (int i = 0; i < HEIGHT; i++) {
    move(i + 1, 2);
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
  }
  refresh();
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
      case ' ':
        userInput(Action, false);
        break;
      case 'e':
        userInput(Pause, false);
        break;
      case 'r':
        userInput(Start, false);
        break;
    }
  }
  return game;
}

int main(void) {
  init_curses();
  print_border();

  int game = 1;
  while (game) {
    GameInfo_t game_info = updateCurrentState();
    print_field(game_info);
    usleep(10000);  // 1/100 sec
    game = handle_user_input();
  }

  endwin();
  return 0;
}