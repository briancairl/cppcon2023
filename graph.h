#include <cctype>
#include <vector>

#include "allocator.h"

struct Adjacency
{
  std::size_t edge_id = 0;
  std::size_t dst_vertex_id = 0;
};

struct VertexProperties
{
  float x = 0.f;
  float y = 0.f;
};

struct EdgeProperties
{
  std::size_t cost = 1;
};

struct Graph
{
  template<typename T>
  using Vector = std::vector<T, Allocator<T>>;

  Vector<Vector<Adjacency>> adjacencies;
  Vector<VertexProperties> vertices;
  Vector<EdgeProperties> edges;

  explicit Graph(std::size_t vertex_count, std::size_t edge_count)
  {
    vertices.resize(vertex_count);
    adjacencies.resize(vertex_count);
    for (auto& adj : adjacencies)
    {
      adj.reserve(edge_count);
    }
  }

  bool has_edge(std::size_t src_vertex_id, std::size_t dst_vertex_id) const
  {
    const auto& c = adjacencies[src_vertex_id];
    return std::any_of(c.begin(), c.end(), [dst_vertex_id](const Adjacency& adj) { return adj.dst_vertex_id == dst_vertex_id; });
  }

  void add_edge(std::size_t src_vertex_id, std::size_t dst_vertex_id, const EdgeProperties& edge_props)
  {
    const auto edge_id = edges.size();
    adjacencies[src_vertex_id].emplace_back(Adjacency{
      .edge_id = edge_id,
      .dst_vertex_id = dst_vertex_id
    });
    edges.emplace_back(edge_props);
  }
};
