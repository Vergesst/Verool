#pragma once
#include <cstddef>
#include <iostream>

#include "../mem_pool/allocator/array_allocator.h"
#include "object_pool.h"
using namespace verool;
const int max_size = 4;
class A {
private:
  typedef ObjectPool<A, ArrayAllocator<A, max_size>> ObjectPool;

  // Object Pool
  static ObjectPool pool;

public:
  A() { std::cout << "construct" << std::endl; }

  ~A() { std::cout << "destructed" << std::endl; }

  void *operator new(size_t n) {
    std::cout << "new" << std::endl;
    return pool.allocate(n);
  }

  void operator delete(void *p) {
    std::cout << "delete" << std::endl;
    pool.deallocate(p);
  }
};
// initialize static varialbe
A::ObjectPool A::pool;