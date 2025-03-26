#include "cache_manager.h"
#include <cassert>

CacheManager::CacheManager(int latency) : MEMORY_LATENCY(latency) {}

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
  if (!evicted.valid) {
    return latency;
  }
  if (index + 1 < caches.size()) {
    uint64_t evictedAddress =
        ((evicted.tag * caches[index]->getNumSets()) + evicted.index) *
        caches[index]->getBlockSize();
    return latency + write(evictedAddress, index + 1);
  }
  if (evicted.dirty) {
    return latency + MEMORY_LATENCY;
  }
  return latency;
}

int CacheManager::access(uint64_t address) {
  int latency = 0;
  for (size_t i = 0; i < caches.size(); i++) {
    CacheBlock *block = caches[i]->access(address);
    if (block) {
      return latency;
    }
    latency += caches[i]->latency;
  }
  return latency + MEMORY_LATENCY + write(address, 0);
}

void CacheManager::clearAll() {
  for (auto cache : caches) {
    cache->clearCache();
  }
}
