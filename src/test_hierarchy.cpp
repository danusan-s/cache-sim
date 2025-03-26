#include "cache_manager.h"
#include "test.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>

// Helper function to print cache statistics
void printCacheStats(const Cache &cache) {
  uint64_t hitCount = cache.getHitCount();
  uint64_t missCount = cache.getMissCount();
  double hitRate = 100.0 * hitCount / (hitCount + missCount);
  std::cout << "Cache Statistics:\n";
  std::cout << "  Hits: " << std::dec << hitCount << "\n";
  std::cout << "  Misses: " << std::dec << missCount << "\n";
  std::cout << "  Hit Rate: " << hitRate << "%\n";
}

// Test random access pattern
int testUniformRandomAccess(CacheManager &cacheMan, int seed = 42) {
  std::cout << "Running Uniform Random access test. 10000 accesses\n";
  // Mersenne Twister random number generator
  // (low chaces of patterns, higher randomness)
  std::mt19937 generator(seed);
  // Uniform distribution for truly random access
  std::uniform_int_distribution<uint64_t> distribution(0x0000, 0xFFFF);

  int latency = 0;

  for (int i = 0; i < 10000; i++) {
    uint64_t randomAddress = distribution(generator);
    latency += cacheMan.access(randomAddress);
  }
  return latency;
}

// Test localized random access pattern
int testLocalizedRandomAccess(CacheManager &cacheMan, int seed = 42) {

  std::cout << "Running Localized random access test. 10000 accesses\n";
  // Minstd random number generator
  // (higher chances of patterns, worse randomness)
  std::minstd_rand generator(seed);
  // Normal distribution for localized random access
  std::normal_distribution<double> distribution(32768.0,
                                                8192.0); // Mean=32K, Stddev=8K

  int latency = 0;

  for (int i = 0; i < 10000; i++) {
    uint64_t randomAddress = static_cast<uint64_t>(
        std::clamp(distribution(generator), 0.0, 65535.0));
    latency += cacheMan.access(randomAddress);
  }
  return latency;
}

int testLateAccess(CacheManager &cacheMan) {
  std::cout << "Running Sequential access test. 2000 accesses\n";

  int latency = 0;

  // Access and ignore for a while and access later
  uint64_t baseAddress = 0x1000;
  for (size_t i = 0; i < 1000; i++) {
    uint64_t address = baseAddress + i * BLOCK_SIZE;
    latency += cacheMan.access(address);
  }
  for (size_t i = 0; i < 1000; i++) {
    uint64_t address = baseAddress + i * BLOCK_SIZE;
    latency += cacheMan.access(address);
  }

  return latency;
}

void test_multiple_cache(int seed) {
  std::cout << "Running multiple cache test\n";
  // Initialize cache
  CacheManager cacheManager(100);

  // Create a hierarchy of caches
  Cache cache1(CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY, POLICY, 4);
  Cache cache2(CACHE_SIZE * 4, BLOCK_SIZE, ASSOCIATIVITY, POLICY, 10);

  // Add caches to cache manager
  cacheManager.addCache(&cache1);
  cacheManager.addCache(&cache2);

  // Run tests
  int latency = testUniformRandomAccess(cacheManager, seed);
  std::cout << "Latency: " << latency << std::endl;
  for (Cache *cache : cacheManager.getCaches()) {
    printCacheStats(*cache);
  }
  cacheManager.clearAll();

  latency = testLocalizedRandomAccess(cacheManager, seed);
  std::cout << "Latency: " << latency << std::endl;
  for (Cache *cache : cacheManager.getCaches()) {
    printCacheStats(*cache);
  }
  cacheManager.clearAll();

  latency = testLateAccess(cacheManager);
  std::cout << "Latency: " << latency << std::endl;
  for (Cache *cache : cacheManager.getCaches()) {
    printCacheStats(*cache);
  }
  cacheManager.clearAll();

  std::cout << "All cache tests completed successfully!\n";
}
