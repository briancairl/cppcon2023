#include <algorithm>
#include <filesystem>

#include "cppcon/common.h"

namespace cppcon::v4
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
  Vector<Vector<std::pair<vertex_id_t, edge_weight_t>>> adjacencies_;
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
    return visited_.count(query_vertex_id);
  }

  UnorderedMap<vertex_id_t, vertex_id_t> visited_;
  MinSortedQueue<Transition> queue_;
  Vector<Transition> queue_back_buffer_;
};

}  // namespace cppcon::v4