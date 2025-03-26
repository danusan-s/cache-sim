// my_functions.h
#ifndef TEST_H
#define TEST_H

#include "cache.h"

void test_single_cache(int seed);
void test_multiple_cache(int seed);

// Test parameters
const size_t CACHE_SIZE = 32 * 1024; // 32 KB
const size_t BLOCK_SIZE = 64;        // 64 bytes per block
const size_t ASSOCIATIVITY = 4;      // 4-way set associative
const ReplacementPolicy POLICY = RANDOM;

// Helper function to print cache statistics
void printCacheStats(const Cache &cache);
#endif
