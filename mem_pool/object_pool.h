#pragma once

#include <cstddef>
#include <new>

namespace verool {
template <typename T, typename Allocator>
class ObjectPool {
private:
  Allocator mem_allocator;

public:
  ObjectPool() = default;

  ~ObjectPool() = default;

  void *allocate(size_t size) {
    if (sizeof(T) != size)
      throw std::bad_alloc();

    return mem_allocator.allocate();
  }

  void deallocate(void *p) {
    mem_allocator.deallocate(static_cast<T *>(p));
  }
}; 
}