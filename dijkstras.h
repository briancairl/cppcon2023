#include <cctype>
#include <limits>

#include "allocator.h"

struct Graph;

struct Visited
{
  std::size_t dst_vertex_id = 0;
  std::size_t cost = std::numeric_limits<std::size_t>::max();
};

constexpr bool operator>(const Visited& lhs, const Visited& rhs)
{
  return lhs.cost > rhs.cost;
}

constexpr bool operator<(const Visited& lhs, const Visited& rhs)
{
  return lhs.cost < rhs.cost;
}

#if defined(DIJKSTRAS_VERSION_1)

#include <functional>
#include <queue>
#include <vector>

class Dijkstras
{
public:
  bool search(const Graph& graph, const std::size_t src_vertex_id, const std::size_t dst_vertex_id);

private:
  bool is_unvisited(std::size_t vertex_id) const
  {
    return visited_[vertex_id].cost == std::numeric_limits<std::size_t>::max();
  }

  template<typename T>
  using Vector = std::vector<T, Allocator<T>>;

  Vector<Visited> visited_;
  std::priority_queue<Visited, Vector<Visited>, std::greater<Visited>> transition_queue_;
};

#elif defined(DIJKSTRAS_VERSION_2)

#include <queue>
#include <vector>

class Dijkstras
{
public:
  bool search(const Graph& graph, const std::size_t src_vertex_id, const std::size_t dst_vertex_id);

private:
  bool is_unvisited(std::size_t vertex_id) const
  {
    return visited_[vertex_id].cost == std::numeric_limits<std::size_t>::max();
  }

  template<typename T>
  using Vector = std::vector<T, Allocator<T>>;

  Vector<Visited> visited_;
  Vector<Visited> just_visited_;
  Vector<Visited> transition_queue_;
};

#endif