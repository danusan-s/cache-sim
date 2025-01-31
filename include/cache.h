#include "cache_block.h"
#include <cstdint>
#include <vector>

enum ReplacementPolicy { LRU, FIFO, RANDOM };

// Data is not actually simulated as we only want to know latency of hit/miss
// behaviour
class Cache {
private:
  /*
   * cacheSize: The total size of the cache in bytes.
   * blockSize: The size of each block in bytes.
   * associativity: The number of blocks in each set (1 for direct-mapped, >1
   * for set-associative).
   * numSets: The number of sets in the cache (calculated in the constructor).
   * replacement_policy: The replacement policy to use when evicting a block
   * cache: The actual cache data structure. A vector of lists of CacheBlocks.
   */
  size_t cacheSize, blockSize, associativity, numSets;
  ReplacementPolicy replacement_policy;
  std::vector<std::vector<CacheBlock>> cache;
  int counter;
  uint64_t hitCount;
  uint64_t missCount;

public:
  /*
   * Constructor for Cache class.
   * Initializes the cache with the given parameters.
   * numSets is calculated as cacheSize / (blockSize * associativity).
   * 1 block per set for direct-mapped, >1 blocks per set for set-associative.
   */
  Cache(size_t cacheSize, size_t blockSize, size_t associativity,
        ReplacementPolicy policy);

  /*
   * Reads data from the cache at the given address.
   * returns true if cache hit, false if cache miss.
   */
  bool read(uint64_t address);

  /*
   * Writes data to the cache at the given address.
   * If cache hit, set dirty and update data.
   * If cache miss, evict block according to replacement policy and load new
   */
  bool write(uint64_t address);

  /*
   * Evicts a block from the given set according to the replacement policy.
   * Returns the evicted block.
   */
  CacheBlock *evictBlock(uint64_t setIndex);

  /*
   * Returns the total number of cache hits.
   */
  uint64_t getHitCount() const { return hitCount; }

  /*
   * Returns the total number of cache misses.
   */
  uint64_t getMissCount() const { return missCount; }

  /*
   * Clear the cache by setting all blocks to invalid
   */
  void clearCache() {
    for (size_t i = 0; i < numSets; i++) {
      for (size_t j = 0; j < associativity; j++) {
        cache[i][j].setValid(false);
      }
    }
  }
};
