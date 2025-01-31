#include "cache_block.h"

CacheBlock::CacheBlock() {
  lruCounter = 0;
  fifoCounter = 0;
  tag = 0;
  valid = false;
  dirty = false;
}

void CacheBlock::setDirty(bool dirty) { this->dirty = dirty; }

void CacheBlock::setValid(bool valid) { this->valid = valid; }

void CacheBlock::setTag(uint64_t tag) { this->tag = tag; }

void CacheBlock::setLRUCounter(int counter) { lruCounter = counter; }

void CacheBlock::setFifoCounter(int counter) { fifoCounter = counter; }
