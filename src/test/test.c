#include <check.h>
#include <stdbool.h>
#include <string.h>

#include "tetris.h"

#define WIDTH 10
#define HEIGHT 20

static void copy_field(int **src, int *dst) {
  for (int i = 0; i < HEIGHT; i++) {
    memcpy(dst + i, src[i], WIDTH * sizeof(int));
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
}
END_TEST

int main(void) {
  SRunner *sr = srunner_create(NULL);

  Suite *s = suite_create("tetris");
  srunner_add_suite(sr, s);

  TCase *tc = tcase_create("test_down");
  suite_add_tcase(s, tc);

  tcase_add_test(tc, test_down);

  srunner_run_all(sr, CK_NORMAL);
  return 0;
}
