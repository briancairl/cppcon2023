#pragma once

#include <cctype>

namespace
{
  constexpr std::size_t kBigBlockOfMemorySize = 10000000;
  std::byte* sBigBlockOfMemory = nullptr;
  std::size_t sBigBlockOfMemoryIndex = 0;
}  // namespace

template<typename T>
class arena_allocator
{
public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using propagate_on_container_move_assignment = std::true_type;

  arena_allocator()
  {
    if (sBigBlockOfMemory == nullptr)
    {
      sBigBlockOfMemory = reinterpret_cast<std::byte*>(std::malloc(kBigBlockOfMemorySize));
    }
  }

  [[nodiscard]] constexpr T* allocate(std::size_t n)
  {
    if (sBigBlockOfMemoryIndex + n > kBigBlockOfMemorySize)
    {
      std::abort();
    }
    T* p = reinterpret_cast<T*>(sBigBlockOfMemory + sBigBlockOfMemoryIndex);
    sBigBlockOfMemoryIndex += n * sizeof(T);
    return p;
  }

  static constexpr void deallocate( T* p, std::size_t n ) {}
};

void reset_arena_memory()
{
  sBigBlockOfMemoryIndex = 0;
}
