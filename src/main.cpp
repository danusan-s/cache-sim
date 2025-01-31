#include "cache.h"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>

// Test parameters
const size_t CACHE_SIZE = 32 * 1024; // 32 KB
const size_t BLOCK_SIZE = 64;        // 64 bytes per block
const size_t ASSOCIATIVITY = 4;      // 4-way set associative
const ReplacementPolicy POLICY = LRU;

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

// Test basic cache operations
void testBasicOperations(Cache &cache) {
  uint64_t testAddress1 = 0x1000;
  uint64_t testAddress2 = 0x2000;

  // Initial read (should be a miss)
  assert(!cache.read(testAddress1));
  std::cout << "Read miss: " << std::hex << testAddress1 << "\n";

  // Writing to cache (should be a hit)
  assert(cache.write(testAddress1));
  std::cout << "Write hit: " << std::hex << testAddress1 << "\n";

  // Read after write (should be a hit)
  assert(cache.read(testAddress1));
  std::cout << "Read hit: " << std::hex << testAddress1 << "\n";

  // Write a different address (should be a miss)
  assert(!cache.write(testAddress2));
  std::cout << "Write miss: " << std::hex << testAddress2 << "\n";

  // Read the second address (should be a hit after the previous write)
  assert(cache.read(testAddress2));
  std::cout << "Read hit: " << std::hex << testAddress2 << "\n";
}

// Test cache eviction and replacement policy
void testEviction(Cache &cache) {
  // Fill the cache with addresses that map to the same set
  size_t numBlocks = CACHE_SIZE / BLOCK_SIZE;
  size_t numSets = numBlocks / ASSOCIATIVITY;

  uint64_t baseAddress = 0x1000;
  for (size_t i = 0; i < ASSOCIATIVITY; i++) {
    uint64_t address = baseAddress + i * BLOCK_SIZE * numSets;
    assert(!cache.read(address)); // Should miss initially
    std::cout << "Read miss (filling cache): " << std::hex << address << "\n";
  }

  // Access one more address to trigger eviction
  uint64_t evictionAddress = baseAddress + ASSOCIATIVITY * BLOCK_SIZE * numSets;
  assert(
      !cache.read(evictionAddress)); // Should miss and evict an existing block
  std::cout << "Read miss (eviction): " << std::hex << evictionAddress << "\n";
}

// Test random access pattern
void testRandomAccess(Cache &cache) {
  std::default_random_engine generator;
  std::uniform_int_distribution<uint64_t> distribution(0x0000, 0xFFFF);

  for (int i = 0; i < 10000; i++) {
    uint64_t randomAddress = distribution(generator) * BLOCK_SIZE;
    if (i % 2 == 0) {
      cache.read(randomAddress);
    } else {
      cache.write(randomAddress);
    }
  }

  std::cout << "Random access test completed.\n";
}

int main() {
  // Initialize cache
  Cache cache(CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY, POLICY);

  // Run tests
  testBasicOperations(cache);
  cache.clearCache();
  testEviction(cache);
  cache.clearCache();
  testRandomAccess(cache);

  // Print cache statistics
  printCacheStats(cache);

  std::cout << "All cache tests completed successfully!\n";
  return 0;
}
