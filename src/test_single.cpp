#include "cache.h"
#include "test.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>

// Test basic cache operations
void testBasicOperations(Cache &cache) {
  uint64_t testAddress1 = 0x1000;
  uint64_t testAddress2 = 0x2000;

  // Initial read (should be a miss)
  assert(!cache.access(testAddress1));
  std::cout << "Read miss: " << std::hex << testAddress1 << "\n";

  cache.write(testAddress1);

  // Read after write (should be a hit)
  assert(cache.access(testAddress1));
  std::cout << "Read hit: " << std::hex << testAddress1 << "\n";

  cache.write(testAddress2);

  // Read the second address (should be a hit after the previous write)
  assert(cache.access(testAddress2));
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
    assert(!cache.access(address)); // Should miss initially
    std::cout << "Read miss (filling cache): " << std::hex << address << "\n";
  }

  // Access one more address to trigger eviction
  uint64_t evictionAddress = baseAddress + ASSOCIATIVITY * BLOCK_SIZE * numSets;
  assert(!cache.access(
      evictionAddress)); // Should miss and evict an existing block
  std::cout << "Read miss (eviction): " << std::hex << evictionAddress << "\n";
}

// Test random access pattern
void testUniformRandomAccess(Cache &cache, int seed = 42) {
  std::cout << "Running Uniform Random access test.\n";
  // Mersenne Twister random number generator
  // (low chaces of patterns, higher randomness)
  std::mt19937 generator(seed);
  // Uniform distribution for truly random access
  std::uniform_int_distribution<uint64_t> distribution(0x0000, 0xFFFF);

  for (int i = 0; i < 10000; i++) {
    uint64_t randomAddress = distribution(generator);
    if (i % 2 == 0) {
      cache.access(randomAddress);
    } else {
      cache.write(randomAddress);
    }
  }
}

// Test localized random access pattern
void testLocalizedRandomAccess(Cache &cache, int seed = 42) {

  std::cout << "Running Localized random access test.\n";
  // Minstd random number generator
  // (higher chances of patterns, worse randomness)
  std::minstd_rand generator(seed);
  // Normal distribution for localized random access
  std::normal_distribution<double> distribution(32768.0,
                                                8192.0); // Mean=32K, Stddev=8K

  for (int i = 0; i < 10000; i++) {
    uint64_t randomAddress = static_cast<uint16_t>(
        std::clamp(distribution(generator), 0.0, 65535.0));
    if (i % 2 == 0) {
      cache.access(randomAddress);
    } else {
      cache.write(randomAddress);
    }
  }
}

void test_single_cache(int seed) {
  // Initialize cache
  Cache cache(CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY, POLICY, 10);

  // Run tests
  testBasicOperations(cache);
  cache.clearCache();

  testEviction(cache);
  cache.clearCache();

  testUniformRandomAccess(cache, seed);
  printCacheStats(cache);
  cache.clearCache();

  testLocalizedRandomAccess(cache, seed);
  printCacheStats(cache);
  cache.clearCache();

  std::cout << "All single cache tests completed successfully!\n" << std::endl;
}
