#include <check.h>
#include <stdbool.h>
#include <string.h>

#include "tetris.h"

#define WIDTH 10
#define HEIGHT 20

static void copy_field(int **src, int *dst) {
  for (int i = 0; i < HEIGHT; i++) {
    memcpy(dst + i * WIDTH, src[i], WIDTH * sizeof(int));
  }
}

static bool cmp_fields(int *f1, int *f2) {
  bool res = true;
  for (int i = 0; res && i < WIDTH * HEIGHT; i++) {
    res = (f1[i] == f2[i]);
  }
  return res;
}

START_TEST(test_down) {
  userInput(Start, false);
  GameInfo_t gameinfo = updateCurrentState();
  int fieldbefore[HEIGHT * WIDTH];
  copy_field(gameinfo.field, fieldbefore);

  userInput(Down, false);
  gameinfo = updateCurrentState();
  int fieldafter[HEIGHT * WIDTH];
  copy_field(gameinfo.field, fieldafter);

  ck_assert_int_eq(cmp_fields(fieldbefore, fieldafter), 0);
  userInput(Terminate, false);
}
END_TEST

START_TEST(test_left) {
  userInput(Start, false);
  GameInfo_t gameinfo = updateCurrentState();
  int fieldbefore[HEIGHT * WIDTH];
  copy_field(gameinfo.field, fieldbefore);

  userInput(Left, false);
  gameinfo = updateCurrentState();
  int fieldafter[HEIGHT * WIDTH];
  copy_field(gameinfo.field, fieldafter);

  ck_assert_int_eq(cmp_fields(fieldbefore, fieldafter), 0);
  userInput(Terminate, false);
}
END_TEST

START_TEST(test_right) {
  userInput(Start, false);
  GameInfo_t gameinfo = updateCurrentState();
  int fieldbefore[HEIGHT * WIDTH];
  copy_field(gameinfo.field, fieldbefore);

  userInput(Right, false);
  gameinfo = updateCurrentState();
  int fieldafter[HEIGHT * WIDTH];
  copy_field(gameinfo.field, fieldafter);

  ck_assert_int_eq(cmp_fields(fieldbefore, fieldafter), 0);
  userInput(Terminate, false);
}
END_TEST

START_TEST(test_commit) {
  userInput(Start, false);
  GameInfo_t gameinfo = updateCurrentState();
  int fieldbefore[HEIGHT * WIDTH];
  copy_field(gameinfo.field, fieldbefore);

  for (int i = 0; i < 19; i++) userInput(Down, false);
  gameinfo = updateCurrentState();
  int fieldafter[HEIGHT * WIDTH];
  copy_field(gameinfo.field, fieldafter);

  ck_assert_int_eq(cmp_fields(fieldbefore, fieldafter), 0);
  userInput(Terminate, false);
}
END_TEST

START_TEST(test_pause) {
  userInput(Start, false);
  for (int i = 0; i < 19; i++) userInput(Down, false);

  userInput(Pause, false);

  GameInfo_t gameinfo = updateCurrentState();
  ck_assert_int_eq(gameinfo.pause, 1);

  int fieldbefore[HEIGHT * WIDTH];
  copy_field(gameinfo.field, fieldbefore);

  userInput(Down, false);
  userInput(Left, false);
  userInput(Right, false);
  userInput(Action, false);

  gameinfo = updateCurrentState();
  int fieldafter[HEIGHT * WIDTH];
  copy_field(gameinfo.field, fieldafter);

  ck_assert_int_ne(cmp_fields(fieldbefore, fieldafter), 0);

  userInput(Pause, false);
  userInput(Down, false);
  gameinfo = updateCurrentState();
  copy_field(gameinfo.field, fieldafter);

  ck_assert_int_eq(cmp_fields(fieldbefore, fieldafter), 0);

  userInput(Terminate, false);
}
END_TEST

int main(void) {
  SRunner *sr = srunner_create(NULL);

  Suite *s = suite_create("tetris");
  srunner_add_suite(sr, s);

  TCase *tc = tcase_create("test_tetris");
  suite_add_tcase(s, tc);

  tcase_add_test(tc, test_down);
  tcase_add_test(tc, test_left);
  tcase_add_test(tc, test_right);
  tcase_add_test(tc, test_commit);
  tcase_add_test(tc, test_pause);

  srunner_run_all(sr, CK_NORMAL);
  return 0;
}
