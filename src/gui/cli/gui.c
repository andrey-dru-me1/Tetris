#include <curses.h>
#include <locale.h>
#include <unistd.h>

#include "brick_game/tetris/tetris.h"

#define WIDTH 10
#define HEIGHT 20

#define SET_SPACE bkgdset(COLOR_PAIR(8))
#define SET_BORDER bkgdset(COLOR_PAIR(9))
#define HIGHLIGHT_TEXT bkgdset(COLOR_PAIR(10))
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
  init_pair(2, COLOR_WHITE, COLOR_RED);
  init_pair(3, COLOR_WHITE, COLOR_GREEN);
  init_pair(4, COLOR_WHITE, COLOR_YELLOW);
  init_pair(5, COLOR_WHITE, COLOR_BLUE);
  init_pair(6, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(7, COLOR_WHITE, COLOR_CYAN);
  init_pair(8, COLOR_YELLOW, COLOR_BLACK);
  init_pair(9, COLOR_WHITE, COLOR_GREEN);
  init_pair(10, COLOR_BLACK, COLOR_WHITE);
}

static void print_border() {
  move(0, 0);
  addch(ACS_ULCORNER);
  for (int i = 0; i < WIDTH * 2; i++) addch(ACS_HLINE);
  addch(ACS_URCORNER);
  move(HEIGHT + 1, 0);
  addch(ACS_LLCORNER);
  for (int i = 0; i < WIDTH * 2; i++) addch(ACS_HLINE);
  addch(ACS_LRCORNER);

  for (int i = 1; i < HEIGHT + 1; i++) {
    mvaddch(i, 0, ACS_VLINE);
    mvaddch(i, WIDTH * 2 + 1, ACS_VLINE);
  }
}

static void print_field(GameInfo_t gameinfo) {
  for (int i = 0; i < HEIGHT; i++) {
    move(i + 1, 1);
    for (int j = 0; j < WIDTH; j++) {
      if (gameinfo.field[i][j] > 0) {
        bkgdset(COLOR_PAIR(gameinfo.field[i][j]));
        addch('[');
        addch(']');
      } else if (gameinfo.field[i][j] == 0) {
        SET_SPACE;
        addstr("  ");
      } else {
        SET_SPACE;
        addch(ACS_BOARD);
        addch(ACS_BOARD);
      }
    }
    RESET_COLOR;
  }
}

static void print_pause() {
  HIGHLIGHT_TEXT;
  mvaddstr(HEIGHT / 2, WIDTH / 2 + 3, "Paused");
  RESET_COLOR;
}

static void print_next(GameInfo_t gameinfo) {
  mvaddstr(1, WIDTH * 2 + 4, "Next");
  for (int i = 0; i < 4; i++) {
    move(i + 2, WIDTH * 2 + 4);
    for (int j = 0; j < 4; j++) {
      if (gameinfo.next[i][j]) {
        bkgdset(COLOR_PAIR(gameinfo.next[i][j]));
        addch('[');
        addch(']');
      } else {
        addstr("  ");
      }
      RESET_COLOR;
    }
  }
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
    GameInfo_t gameinfo = updateCurrentState();
    print_field(gameinfo);
    print_next(gameinfo);
    if (gameinfo.pause) print_pause();
    refresh();
    usleep(10000);  // 1/100 sec
    game = handle_user_input();
  }

  endwin();
  return 0;
}