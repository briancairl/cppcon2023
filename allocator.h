#ifdef USE_ARENA_ALLOCATOR

#include "arena_allocator.h"

template<typename T>
using Allocator = arena_allocator<T>;

#else  // USE_ARENA_ALLOCATOR

#include <memory>

template<typename T>
using Allocator = std::allocator<T>;

#endif  // USE_ARENA_ALLOCATOR
