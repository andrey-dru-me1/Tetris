#include "brick_game/tetris/types/blockarray.h"

#include <stdlib.h>

void blockarray_create(blockarray_t *blockarray, size_t n) {
  blockarray->blocks = calloc(n, sizeof(block_t));
  blockarray->n = n;
}

block_t blockarray_get(blockarray_t *blockarray, size_t i) {
  return blockarray->blocks[i];
}

void blockarray_set(blockarray_t *blockarray, size_t i, block_t val) {
  blockarray->blocks[i] = val;
}

void blockarray_remove(blockarray_t *blockarray) { free(blockarray->blocks); }
