#include <algorithm>
#include <ranges>
#include <filesystem>

#include "cppcon/common.h"

namespace cppcon::v6
{

class Graph
{
public:
  [[nodiscard]] static Graph read(const std::filesystem::path& graph_file_name);

  std::size_t vertex_count() const { return vertices_.size(); }

  template<typename EdgeVisitorT>
  void for_each_edge(vertex_id_t parent_vertex_id, EdgeVisitorT visitor) const;

private:
  Vector<VertexProperties> vertices_;

  using Edge = std::pair<vertex_id_t, edge_weight_t>;

  Vector<std::ranges::subrange<const Edge*>> adjacencies_;
  Vector<Edge> edges_;
};

class Dijkstras
{
public:
  explicit Dijkstras(const Graph& graph);

  bool search(const Graph& graph, vertex_id_t src_vertex_id, vertex_id_t dst_vertex_id);

  Vector<vertex_id_t> get_path(vertex_id_t dst_vertex_id) const;

private:
  bool is_visited(vertex_id_t query_vertex_id) const
  {
    return visited_[query_vertex_id] != visited_.size();
  }

  Vector<vertex_id_t> visited_;
  MinSortedQueue<Transition> queue_;
  Vector<Transition> queue_back_buffer_;
};

}  // namespace cppcon::v6
