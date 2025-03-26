#ifndef CACHE_MANAGER_H
#define CACHE_MANAGER_H

#include "cache.h"
#include <cstdint>
#include <vector>

// Keep things simple for now
// Implement a write-back policy with write-allocate
// where we write to mem on eviction and allocate after write miss
// Other option is write-through with no-write-allocate
// which is the opposite what we are doing here
// Use a Non Inclusive Non Exclusive (NINE) hierarchy of caches

class CacheManager {
private:
  std::vector<Cache *> caches;
  int MEMORY_LATENCY;

public:
  CacheManager(int latency = 100);

  void addCache(Cache *cache);
  void removeCache(size_t index);
  int write(uint64_t address, size_t index);
  int access(uint64_t address);
  std::vector<Cache *> getCaches() { return caches; }
  void clearAll();
  void printAll();
};

#endif // CACHE_MANAGER_H
