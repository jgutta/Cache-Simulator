#include "cpu.h"
#include "cache.h"
#include "lru.h"
#include "bits.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char* make_blocks(int block_count) {
  /* Makes and initializes the blocks given the block_count.
   */
  char* blok = (char*)malloc((block_count)*sizeof(char));
  return blok;
}

Line* make_lines(int line_count, int block_count) {
  /* Makes and initializes the lines given the line count. Then
     makes and initializes the blocks.
   */
    Line* lines = (Line*)malloc((line_count)*sizeof(Line));
    for(int i = 0; i < line_count; i++){
    (lines[i].block_count) = block_count;
    (lines[i].tag) = NULL;
    (lines[i].valid) = 0;
    (lines[i].blocks) = make_blocks(block_count);
    }

  return lines;
}

Set* make_sets(int set_count, int line_count, int block_count) {
  /* Makes and initializes the sets given the set count. Then
     makes and initializes the lines and blocks.
   */
    Set* set = (Set*)malloc((set_count)* sizeof(Set));
    for(int i = 0; i < set_count; i++){
    set[i].line_count = line_count;
    set[i].lines = make_lines(line_count, block_count);
    }
  return set;
}

Cache* make_cache(int set_bits, int line_count, int block_bits) {
  Cache* cache = NULL;
  //Makes and initializes the cache, sets, lines, and blocks.


  cache = (Cache*)malloc(sizeof(Cache));
  cache->set_bits = set_bits;
  cache->line_count = line_count;
  cache->block_bits = block_bits;
  cache->set_count = exp2(set_bits);
  cache->block_count = exp2(block_bits);
  cache->sets = make_sets((cache->set_count), line_count, (cache->block_count));

  // Create LRU queues for sets:
  if (cache != NULL)
    lru_init(cache);

  return cache;
}

void delete_blocks(char* blocks) {
  free(blocks);
}

void delete_lines(Line* lines, int line_count) {
  for (int i = 0; i < line_count; i++) {
    delete_blocks(lines[i].blocks);
  }
  free(lines);
}

void delete_sets(Set* sets, int set_count) {
  for (int i = 0; i < set_count; i++) {
    delete_lines(sets[i].lines, sets[i].line_count);
  }
  free(sets);
}

void delete_cache(Cache* cache) {
  lru_destroy(cache);
  delete_sets(cache->sets, cache->set_count);
  free(cache);
}

AccessResult cache_access(Cache* cache, TraceLine* trace_line) {
  unsigned int s = get_set(cache, trace_line->address);
  unsigned int t = get_line(cache, trace_line->address);
  unsigned int b = get_block(cache, trace_line->address);

  // Get the set:
  Set* set   = &cache->sets[s];

  // Get the line:
  LRUResult result;
  lru_fetch(set, t, &result);
  Line* line = result.line;

  // If it was a miss we will clear the blocks:
  if (result.access != HIT) {
    for (int i = 0; i < cache->block_count; i++) {
      line->blocks[i] = 0;
    }
  }

  // Then set the block to 1:
  line->blocks[b] = 1;

  return result.access;
}
