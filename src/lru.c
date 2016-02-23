#include <stdlib.h>
#include <stdio.h>
#include "cache.h"
#include "lru.h"

void lru_init_queue(Set* set) {
  LRUNode* s = NULL;
  LRUNode* p = NULL;
  for (int i = 0; i < set->line_count; i++) {
    Line* line = &set->lines[i];
    if (p == NULL) {
      p = (LRUNode*) malloc(sizeof(LRUNode));
      p->line = line;
      s = p;
    }
    else {
      p->next = (LRUNode*) malloc(sizeof(LRUNode));
      p = p->next;
      p->line = line;
    }
  }
  set->lru_queue = s;
}

void lru_init(Cache* cache) {
  Set* sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++) {
    lru_init_queue(&sets[i]);
  }
}

void lru_destroy(Cache* cache) {
  Set* sets = cache->sets;
  for (int i = 0; i < cache->set_count; i++) {
    LRUNode* p = sets[i].lru_queue;
    LRUNode* n = p;
    while (p != NULL) {
      p = p->next;
      free(n);
      n = p;
    }
    sets[i].lru_queue = NULL;
  }
}

void lru_fetch(Set* set, unsigned int tag, LRUResult* result) {
  /*Least Recently used algorithm, used to see which line to access first
   */
    LRUNode* current = set -> lru_queue;
    LRUNode* temp = current;


      while(current != NULL){
         Line* line = current -> line;
         if(line -> valid == 1 && tag == line ->tag){
          //Valid, Hit

            if(current != set -> lru_queue){
            temp -> next = current -> next;
            current -> next = set -> lru_queue;
            set -> lru_queue = current;
            }
            result -> access = HIT;
            result -> line = line;
            break;
          }

          else if(line -> valid == 0){
            //Invalid, cold miss
            line -> valid = 1;
            line ->tag = tag;

            if(current != set -> lru_queue){
            temp -> next = current -> next;
            current -> next = set -> lru_queue;
            set-> lru_queue = current;
            }
            result -> access = COLD_MISS;
            result -> line = line;
            break;
          }

          else if(current -> next == NULL){
            //Conflict
            line -> tag = tag;

            if(current != set -> lru_queue){
            temp -> next = current -> next;
            current -> next = set -> lru_queue;
            set -> lru_queue = current;
            }
            result -> access = CONFLICT_MISS;
            result -> line = line;
            break;
          }

          temp = current;
          current = current -> next;
      }
}
