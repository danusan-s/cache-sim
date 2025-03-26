#include "cache_manager.h"
#include <cassert>

CacheManager::CacheManager(int latency) : MEMORY_LATENCY(latency) {}

CacheManager::~CacheManager() {
  for (auto cache : caches) {
    delete cache;
  }
}

void CacheManager::addCache(Cache *cache) {
  if (caches.size() > 0) {
    assert(cache->getBlockSize() == caches.back()->getBlockSize());
  }
  caches.push_back(cache);
}

void CacheManager::removeCache(size_t index) {
  if (index >= caches.size()) {
    return;
  }
  caches.erase(caches.begin() + index);
}

int CacheManager::write(uint64_t address, size_t index) {
  int latency = caches[index]->latency;
  CacheBlock evicted = caches[index]->write(address);
  if (caches.size() > index + 1) {
    uint64_t evictedAddress = evicted.tag * caches[index]->getNumSets() *
                              caches[index]->getBlockSize();
    CacheBlock *existCacheBlock = caches[index + 1]->access(evictedAddress);
    if (existCacheBlock && evicted.dirty) {
      // Overwrite the block in the next level cache
      existCacheBlock->setDirty(true);
      return latency;
    }
    return latency + write(evictedAddress, index + 1);
  }
  if (evicted.dirty) {
    return latency + MEMORY_LATENCY;
  }
  return latency;
}
