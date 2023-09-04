#include <algorithm>
#include <filesystem>
#include <limits>
#include <vector>
#include <queue>
#include <map>
#include <set>

#include "allocator.h"

namespace cppcon
{

template<typename T>
using Vector = std::vector<T, Allocator<T>>;

template<typename T>
using Set = std::set<T, std::less<T>, Allocator<T>>;

template<typename Key, typename T>
using Map = std::map<Key, T, std::less<Key>, Allocator<std::pair<const Key, T>>>;

template<typename Key, typename T>
using MultiMap = std::multimap<Key, T, std::less<Key>, Allocator<std::pair<const Key, T>>>;

template<typename T>
using MinSortedQueue = std::priority_queue<T, Vector<T>, std::greater<T>>;

using edge_id_t = std::uint32_t;

using vertex_id_t = std::uint32_t;

using edge_weight_t = std::size_t;

struct VertexProperties
{
  double x;
  double y;
};

}  // namespace cppcon
