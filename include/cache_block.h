#include <cstdint>
#include <vector>

/*
 * CacheBlock struct represents a block in the cache.
 * tag: The tag of the block which refers to the memory address being cached.
 * valid: A flag indicating whether the block is valid or not (i.e. contains
 * some cache and not random bits).
 * dirty: A flag indicating whether the block
 * has been modified since it was loaded into the cache. (useful for writing
 * back to memory only when evicting from cache)
 * lruCounter: A counter used to implement the LRU replacement policy. (Higher
 * is more recently used)
 */
class CacheBlock {
public:
  int lruCounter;
  int fifoCounter;
  uint64_t tag;
  uint64_t index;
  bool valid;
  bool dirty;

  CacheBlock();

  void setDirty(bool dirty);
  void setValid(bool valid);
  void setTag(uint64_t tag);
  void setIndex(uint64_t setIndex);
  void setLRUCounter(int counter);
  void setFifoCounter(int counter);
};
