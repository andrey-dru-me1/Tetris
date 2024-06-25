#include "brick_game/tetris/game.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "brick_game/tetris/defs.h"
#include "brick_game/tetris/figset.h"

void game_init(game_t *game) {
  game->info.field = calloc(HEIGHT, sizeof(*game->info.field));
  int *field = calloc(WIDTH * HEIGHT, sizeof(*field));
  for (size_t row = 0; row < HEIGHT; row++)
    game->info.field[row] = field + row * WIDTH;
  game->info.next = calloc(4, sizeof(*game->info.next));
  int *next = calloc(4 * 4, sizeof(*next));
  for (size_t i = 0; i < 4; i++) game->info.next[i] = next + i * 4;
  game->info.high_score = 0;
  game->info.pause = 0;
  game->info.level = 0;
  game->info.score = 0;
  game->info.speed = 0;

  FILE *hscorefile = fopen("high_score.txt", "r");
  if (hscorefile) {
    fscanf(hscorefile, "%d", &game->info.high_score);
    fclose(hscorefile);
  } else
    game->info.high_score = 0;

  figset_init(&game->figset);
  blockmatrix_create(&game->field.bm, HEIGHT, WIDTH);
  srand(time(NULL));

  game->nextfig = game->figset.figs + rand() % 7;
  gettimeofday(&game->nexttm, NULL);
}

void game_delete(game_t *game) {
  FILE *file;
  if (game->info.score > game->info.high_score &&
      (file = fopen("high_score.txt", "w"))) {
    fprintf(file, "%d", game->info.score);
    fclose(file);
  }

  free(*game->info.field);
  free(game->info.field);
  game->info.field = NULL;
  free(*game->info.next);
  free(game->info.next);
  game->info.field = NULL;

  blockmatrix_remove(&game->field.bm);
  field_removefig(&game->field);
  figset_free(&game->figset);
}
