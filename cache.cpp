#include <cstdint>
#include <list>
#include <vector>

enum ReplacementPolicy { LRU, FIFO, RANDOM };

class Cache {
private:
  struct CacheBlock {
    uint64_t tag;
  };

  size_t cacheSize, blockSize, associativity, numSets;
  ReplacementPolicy replacement_policy;
  std::vector<std::list<CacheBlock>> cache;

public:
  Cache(size_t cacheSize, size_t blockSize, size_t associativity,
        ReplacementPolicy policy)
      : cacheSize(cacheSize), blockSize(blockSize),
        associativity(associativity), replacement_policy(policy) {
    numSets = cacheSize / (blockSize * associativity);
    cache.resize(numSets);
  }
};
