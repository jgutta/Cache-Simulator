#ifndef __CACHE_H
#define __CACHE_H
#include "trace.h"

// Forward declaration of types:
typedef enum AccessResult AccessResult;
typedef struct LRUNode LRUNode;
typedef struct Line Line;
typedef struct Set Set;
typedef struct Cache Cache;
typedef struct LRUResult LRUResult;

/*** General Cache Data Structures ***/

// This represents a result of accessing the cache.
// Was it a "hit" or a "miss".
enum AccessResult {
  HIT,
  COLD_MISS,
  CONFLICT_MISS
};

/*** LRU Data Structures ***/

// Represents a node in the LRU queue/stack linked list.
struct LRUNode {
  Line*    line;
  LRUNode* next;
};

// Represents the result returned from accessing the LRU queue/stack.
struct LRUResult {
  Line* line;
  AccessResult access;
};

/*** Cache Data Structures ***/

// A Line represents a line in the cache.
struct Line {
  char  valid;          // is the line valid?
  int   tag;            // the tag
  char* blocks;         // the list of blocks
  int   block_count;    // the number of blocks
};

// A Set represents a set in the cache.
struct Set {
  Line*    lines;       // The lines in the set
  int      line_count;  // The number of lines
  LRUNode* lru_queue;   // The LRU queue/stack associated with the set
};

// A Cache represents the state of the cache.
struct Cache {
  Set* sets;            // The sets in the cache
  int  set_count;       // The number of sets
  int  line_count;      // The number of lines
  int  block_count;     // The number of blocks
  int  set_bits;        // The number of bits for the set
  int  block_bits;      // The number of bits used for the block
};

Cache* make_cache(int set_count, int line_count, int block_count);
void delete_cache(Cache* cache);
int get_set(Cache* cache, address_type address);
int get_line(Cache* cache, address_type address);
int get_block(Cache* cache, address_type address);
AccessResult cache_access(Cache* cache, TraceLine* trace_line);

#endif
