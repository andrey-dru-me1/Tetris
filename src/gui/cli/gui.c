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

  init_color(8, 1000, 0, 0);       // RED for I
  init_color(9, 1000, 1000, 0);    // YELLOW for L
  init_color(10, 1000, 396, 8);    // ORANGE for J
  init_color(11, 1000, 600, 796);  // PINK for O
  init_color(12, 0, 0, 1000);      // BLUE for T
  init_color(13, 0, 502, 0);       // BLUE for S
  init_color(14, 502, 0, 502);     // PURPLE for Z

  for (int i = 0; i < 7; i++) {
    init_pair(i + 1, COLOR_BLACK, i + 8);
  }

  init_pair(8, COLOR_YELLOW, COLOR_BLACK);
  init_pair(9, COLOR_WHITE, COLOR_GREEN);
  init_pair(10, COLOR_BLACK, COLOR_WHITE);
}

static void print_border() {
  move(0, 0);
  addch(ACS_ULCORNER);
  for (int i = 0; i < WIDTH * 2; i++) addch(ACS_HLINE);
  addch(ACS_TTEE);
  for (int i = 0; i < 12; i++) addch(ACS_HLINE);
  addch(ACS_URCORNER);

  move(HEIGHT + 1, 0);
  addch(ACS_LLCORNER);
  for (int i = 0; i < WIDTH * 2; i++) addch(ACS_HLINE);
  addch(ACS_BTEE);
  for (int i = 0; i < 12; i++) addch(ACS_HLINE);
  addch(ACS_LRCORNER);

  for (int i = 1; i < HEIGHT + 1; i++) {
    mvaddch(i, 0, ACS_VLINE);
    mvaddch(i, WIDTH * 2 + 1, ACS_VLINE);
    mvaddch(i, WIDTH * 2 + 14, ACS_VLINE);
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
  mvaddstr(HEIGHT / 2, WIDTH - 2, "Paused");
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

static void print_score(GameInfo_t gameinfo) {
  move(7, WIDTH * 2 + 3);
  addstr("Score:");
  move(8, WIDTH * 2 + 3);
  printw("%10d", gameinfo.score);
}

static void print_level(GameInfo_t gameinfo) {
  move(9, WIDTH * 2 + 3);
  addstr("Level:");
  move(10, WIDTH * 2 + 3);
  printw("%10d", gameinfo.score / 600);
}

static void print_description() {
  mvaddstr(12, WIDTH * 2 + 2, "'q' Term");
  mvaddstr(13, WIDTH * 2 + 2, "'w' Up");
  mvaddstr(14, WIDTH * 2 + 2, "'a' Left");
  mvaddstr(15, WIDTH * 2 + 2, "'s' Down");
  mvaddstr(16, WIDTH * 2 + 2, "'d' Right");
  mvaddstr(17, WIDTH * 2 + 2, "' ' Action");
  mvaddstr(18, WIDTH * 2 + 2, "'e' Pause");
  mvaddstr(19, WIDTH * 2 + 2, "'r' Start");
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
  print_description();

  int game = 1;
  while (game) {
    GameInfo_t gameinfo = updateCurrentState();
    print_field(gameinfo);
    print_next(gameinfo);
    print_score(gameinfo);
    print_level(gameinfo);
    if (gameinfo.pause) print_pause();
    refresh();
    usleep(10000);  // 1/100 sec
    game = handle_user_input();
  }

  endwin();
  return 0;
}