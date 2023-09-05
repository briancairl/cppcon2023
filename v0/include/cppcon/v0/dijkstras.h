#include <algorithm>
#include <filesystem>

#include "cppcon/common.h"

namespace cppcon::v0
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
  MultiMap<vertex_id_t, std::pair<vertex_id_t, edge_weight_t>> adjacencies_;
};

class Dijkstras
{
public:
  explicit Dijkstras(const Graph& graph);

  bool search(const Graph& graph, vertex_id_t src_vertex_id, vertex_id_t dst_vertex_id);

  std::vector<vertex_id_t> get_path(vertex_id_t dst_vertex_id) const;

private:
  Map<vertex_id_t, vertex_id_t> visited_;
  MinSortedQueue<Transition> queue_;
};

}  // namespace cppcon::v0
