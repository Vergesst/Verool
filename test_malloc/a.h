#pragma once
#include <cstddef>
#include <iostream>

#include "../mem_pool/allocator/malloc_allocator.h"
#include "object_pool.h"
using namespace verool;

class A {
private:
  typedef ObjectPool<A, MallocAllocator<A>> ObjectPool;

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