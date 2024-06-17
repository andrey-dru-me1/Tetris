#ifndef BITARRAY_H_68415
#define BITARRAY_H_68415

#include <stddef.h>

#include "brick_game/tetris/types/block.h"

typedef struct {
  block_t *blocks;
  size_t n;
} blockarray_t;

void blockarray_create(blockarray_t *blockarray, size_t n);
block_t blockarray_get(blockarray_t *blockarray, size_t i);
void blockarray_set(blockarray_t *blockarray, size_t i, block_t val);
void blockarray_remove(blockarray_t *blockarray);

#endif
