#include <algorithm>
#include <filesystem>

#include "allocator.h"
#include "common.h"

namespace cppcon
{

class Graph
{
public:
  [[nodiscard]] static Graph read(const std::filesystem::path& graph_file_name);

  std::size_t vertex_count() const { return vertices_.size(); }

  template<typename EdgeVisitorT>
  void for_each_edge(vertex_id_t parent_vertex_id, EdgeVisitorT visitor) const
  {
    auto range = adjacencies_.equal_range(parent_vertex_id);
    std::for_each(
      range.first,
      range.second,
      [visitor](const auto& parent_and_edge)
      {
        const auto& [parent_vertex_id, child_vertex_id_and_edge_props] = parent_and_edge;
        const auto& [child_vertex_id, edge_properties] = child_vertex_id_and_edge_props;
        visitor(parent_vertex_id, child_vertex_id, edge_properties);
      });
  }

private:
  Vector<VertexProperties> vertices_;
  MultiMap<vertex_id_t, std::pair<vertex_id_t, edge_weight_t>> adjacencies_;
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

class Dijkstras
{
public:
  explicit Dijkstras(const Graph& graph);

  bool search(const Graph& graph, vertex_id_t src_vertex_id, vertex_id_t dst_vertex_id);

  std::vector<vertex_id_t> get_path(vertex_id_t dst_vertex_id) const;

private:
  Set<vertex_id_t> visited_;
  Map<vertex_id_t, vertex_id_t> parents_;
  MinSortedQueue<Transition> queue_;
};

}  // namespace cppcon
