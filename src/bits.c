#include "cache.h"
#include "bits.h"

int get_set(Cache* cache, address_type address) {
  /* Extracts the set bits from a 32-bit address.
   */
  int newSet = cache -> block_bits;

    address = address >> newSet;

    newSet = 32 - (cache-> set_bits);
    address = address << newSet;
    address = address >> newSet;


  return (int)address;
}

int get_line(Cache* cache, address_type address) {
  /*Extracts the tag bits from a 32-bit address.
   */
  int newLine = cache-> set_bits + cache -> block_bits;

  address = address >> newLine;


  return (int)address;
}

int get_block(Cache* cache, address_type address) {
  /* Extract the block bits from a 32-bit address.
   */
    int newBlock = 32 - cache -> block_bits;

    address = address << newBlock;
    address = address >> newBlock;

  return (int)address;
}
