#pragma once

#include "allocator.h"
#include <cstdlib>

namespace verool {
template <typename T>
class MallocAllocator : public Allocator<T> {
public:
  MallocAllocator() = default;

  ~MallocAllocator() = default;

  virtual T *allocate() {
    auto p = ::malloc(sizeof(T));
    // type cast in runtime
    return reinterpret_cast<T *>(p);
  }

  virtual void deallocate(T *p) {
    ::free(p);
  }
};
}