// #pragma once

// #include "allocator.h"
// #include <cstddef>
// #include <new>

// namespace verool {
// template <typename T, int N>
// class ArrayAllocator : public Allocator<T> {
// private:
//   // size of mem
//   unsigned char m_data[sizeof(T) * N];

//   // check whether is used
//   bool m_used[N];

// public:
//   ArrayAllocator() {
//     for (int i = 0; i < N; i++) {
//       m_used[i] = false;
//     }
//   }

//   ~ArrayAllocator() = default;

//   // implement allocator's methods
//   virtual T *allocate(size_t _) {
//     for (int i = 0; i < N; i++) {
//       if (!m_used[i]) {
//         m_used[i] = true;
//         return reinterpret_cast<T *>(m_data + sizeof(T) * i);
//       }
//     }
//     // no mem to allocate
//     throw std::bad_alloc();
//   }

//   virtual void deallocate(T *p) {
//     auto i = ((unsigned char *)p - m_data) / sizeof(T);

//     m_used[i] = false;
//     // ::free()
//   }
// };
// }

#pragma once

#include "allocator.h" // Ensure this defines Allocator<T> with the correct virtual methods
#include <new>       // For std::bad_alloc
#include <stdexcept> // For other potential exceptions like invalid_argument

namespace verool {

template <typename T, int N> class ArrayAllocator : public Allocator<T> {
private:
  // Raw memory block for N objects of type T
  unsigned char m_data[sizeof(T) * N];
  // Usage flags for each T-sized slot
  bool m_used[N];

public:
  ArrayAllocator() {
    for (int i = 0; i < N; i++) {
      m_used[i] = false; // Initialize all slots as free
    }
  }

  // Default destructor is fine if you're not managing external resources
  ~ArrayAllocator() = default;

  // Implementation of Allocator's allocate method
  // This allocator is designed for single object allocations (n=1)
  // If you need to allocate 'n' contiguous objects, the logic will be
  // different.
  T *allocate() override {

    for (int i = 0; i < N; i++) {
      if (!m_used[i]) {   // Find an UNUSED slot
        m_used[i] = true; // Mark it as USED
        // Calculate the address of the i-th T-sized slot
        return reinterpret_cast<T *>(m_data + (sizeof(T) * i));
      }
    }
    // No free memory slots left
    throw std::bad_alloc();
  }

  // Implementation of Allocator's deallocate method
  void deallocate(T *p) override {
    if (p == nullptr) {
      return; // Deallocating a null pointer is generally a no-op
    }

    // Cast to unsigned char* for byte-level pointer arithmetic
    unsigned char *byte_ptr = reinterpret_cast<unsigned char *>(p);

    // Check if the pointer is within the bounds of this allocator's memory
    if (byte_ptr < m_data || byte_ptr >= (m_data + sizeof(T) * N)) {
      // This pointer doesn't belong to this allocator's memory pool.
      // This is a critical error in a real-world scenario.
      throw std::invalid_argument(
          "Attempt to deallocate memory not owned by this allocator.");
    }

    // Calculate the index based on the offset from the start of m_data
    // Ensure the pointer is aligned to a T-sized boundary
    if ((byte_ptr - m_data) % sizeof(T) != 0) {
      // Pointer is not aligned to a T-sized boundary, likely an error
      throw std::invalid_argument("Attempt to deallocate misaligned pointer.");
    }

    auto i = (byte_ptr - m_data) / sizeof(T);

    if (m_used[i]) {     // Only deallocate if it was actually marked as used
      m_used[i] = false; // Mark the slot as FREE
    } else {
      // Attempting to deallocate memory that was already free or never
      // allocated
      throw std::invalid_argument(
          "Attempt to deallocate free or unallocated memory.");
    }
  }
};

} // namespace verool