#pragma once 

#include <algorithm>
#include <filesystem>
#include <limits>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
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
using UnorderedSet = std::unordered_set<T, std::hash<T>, std::equal_to<T>, Allocator<T>>;

template<typename Key, typename T>
using UnorderedMap = std::unordered_map<Key, T, std::hash<Key>, std::equal_to<Key>, Allocator<std::pair<const Key, T>>>;

template<typename Key, typename T>
using UnorderedMultiMap = std::unordered_multimap<Key, T, std::hash<Key>, std::equal_to<Key>, Allocator<std::pair<const Key, T>>>;

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

struct Transition
{
  vertex_id_t src_vertex_id = 0;
  vertex_id_t dst_vertex_id = 0;
  edge_weight_t total_weight = 0;
};

constexpr bool operator>(const Transition& lhs, const Transition& rhs)
{
  return lhs.total_weight > rhs.total_weight;
}

constexpr bool operator<(const Transition& lhs, const Transition& rhs)
{
  return lhs.total_weight < rhs.total_weight;
}

}  // namespace cppcon
