#include "test.h"
#include <iostream>

int main() {
  std::cout << "Cache Simulator\n";
  std::cout << "Enter a random seed value: ";
  int seed;
  std::cin >> seed;

  test_single_cache(seed);
  test_multiple_cache(seed);
}
