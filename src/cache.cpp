#include "cache.h"
#include <random>

Cache::Cache(size_t cacheSize, size_t blockSize, size_t associativity,
             ReplacementPolicy policy, int latency = 0)
    : cacheSize(cacheSize), blockSize(blockSize), associativity(associativity),
      replacement_policy(policy), counter(0), hitCount(0), missCount(0),
      latency(latency) {
  numSets = cacheSize / (blockSize * associativity);
  cache.resize(numSets, std::vector<CacheBlock>(associativity));
}

CacheBlock *Cache::access(uint64_t address) {
  ++counter;

  // Calculate the set index and tag
  uint64_t setIndex = (address / blockSize) % numSets;
  uint64_t tag = address / (blockSize * numSets);

  // Check if the block is in the cache
  for (size_t i = 0; i < associativity; i++) {
    if (cache[setIndex][i].tag == tag && cache[setIndex][i].valid) {
      // Cache hit
      ++hitCount;
      cache[setIndex][i].setLRUCounter(counter);
      return &cache[setIndex][i];
    }
  }

  ++missCount;
  return nullptr;
}

CacheBlock Cache::write(uint64_t address) {
  ++counter;

  // Calculate the set index and tag
  uint64_t setIndex = (address / blockSize) % numSets;
  uint64_t tag = address / (blockSize * numSets);

  CacheBlock *evictedBlock = evictBlock(setIndex);
  CacheBlock replacedBlock = *evictedBlock;

  evictedBlock->setTag(tag);
  evictedBlock->setIndex(setIndex);
  evictedBlock->setValid(true);
  evictedBlock->setLRUCounter(counter);
  evictedBlock->setFifoCounter(counter);

  return replacedBlock;
}

CacheBlock *Cache::evictBlock(uint64_t setIndex) {
  // Check if there is an empty block in the set
  for (size_t i = 0; i < associativity; i++) {
    if (!cache[setIndex][i].valid) {
      return &cache[setIndex][i];
    }
  }

  // Evict block according to replacement policy
  if (replacement_policy == LRU) {
    // Find the least recently used block
    int maxLRUCounter = cache[setIndex][0].lruCounter;
    CacheBlock *lruBlock = &cache[setIndex][0];
    for (size_t i = 1; i < associativity; i++) {
      if (cache[setIndex][i].lruCounter > maxLRUCounter) {
        maxLRUCounter = cache[setIndex][i].lruCounter;
        lruBlock = &cache[setIndex][i];
      }
    }
    return lruBlock;
  } else if (replacement_policy == FIFO) {
    // Find the first block that was loaded
    int maxFifoCounter = cache[setIndex][0].fifoCounter;
    CacheBlock *fifoBlock = &cache[setIndex][0];
    for (size_t i = 1; i < associativity; i++) {
      if (cache[setIndex][i].fifoCounter > maxFifoCounter) {
        maxFifoCounter = cache[setIndex][i].fifoCounter;
        fifoBlock = &cache[setIndex][i];
      }
    }
    return fifoBlock;
  }

  // Random replacement policy
  std::minstd_rand rng(42);
  std::uniform_int_distribution<int> dist(0, associativity - 1);
  int randomIndex = dist(rng);
  return &(cache[setIndex][randomIndex]);
}
