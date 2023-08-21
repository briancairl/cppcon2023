#include <functional>
#include <limits>
#include <vector>

#include "allocator.h"

template<typename T>
using Vector = std::vector<T, Allocator<T>>;

struct Adjacency
{
  std::uint32_t edge_id = 0;
  std::uint32_t dst_vertex_id = 0;
  std::uint32_t cost = 1;
};

struct VertexProperties
{
  float x = 0.f;
  float y = 0.f;
  float big[32];
};

struct EdgeProperties
{
  std::uint32_t cost = 1;
};

struct Graph
{
  Vector<Vector<Adjacency>> adjacencies;
  Vector<VertexProperties> vertices;
  Vector<EdgeProperties> edges;

  explicit Graph(std::uint32_t vertex_count, std::uint32_t edge_count);

  bool has_edge(std::uint32_t src_vertex_id, std::uint32_t dst_vertex_id) const;

  void add_edge(std::uint32_t src_vertex_id, std::uint32_t dst_vertex_id, const EdgeProperties& edge_props);
};

struct Transition
{
  std::uint32_t dst_vertex_id = 0;
  std::uint32_t cost = std::numeric_limits<std::uint32_t>::max();
  Transition() = default;
  Transition(std::uint32_t _dst_vertex_id, std::uint32_t _cost) : dst_vertex_id{_dst_vertex_id}, cost{_cost} {}
};

constexpr bool operator>(const Transition& lhs, const Transition& rhs)
{
  return lhs.cost > rhs.cost;
}

constexpr bool operator<(const Transition& lhs, const Transition& rhs)
{
  return lhs.cost < rhs.cost;
}

class Dijkstras
{
public:
  bool search(const Graph& graph, const std::uint32_t src_vertex_id, const std::uint32_t dst_vertex_id);

private:
  bool is_unvisited(std::uint32_t vertex_id) const
  {
    return !visited_[vertex_id];
  }

  template<typename T>
  using Vector = std::vector<T, Allocator<T>>;

  Vector<bool> visited_;
  Vector<std::uint32_t> transitions_;
  Vector<Transition> just_visited_;
  Vector<Transition> transition_queue_;
};
