#pragma once

namespace verool {
// interface definition
template <typename T> class Allocator {
public:
  virtual T *allocate() = 0;
  virtual void deallocate(T *p) = 0;
};
}