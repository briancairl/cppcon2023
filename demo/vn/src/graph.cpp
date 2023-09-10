// CppCon
#include <cppcon/demo/vn/graph.h>
#include <cppcon/demo/json.h>

namespace cppcon::demo::vn
{

Graph::Graph(const std::filesystem::path& graph_file_name)
{
  const auto v = load_json(graph_file_name);
  const auto& root = v.get<picojson::object>();
  const auto& nodes = root.at("nodes").get<picojson::array>();

  this->vertices_.reserve(nodes.size());
  for (const auto& node_value : nodes)
  {
    const auto& node_object = node_value.get<picojson::object>();
    this->vertices_.push_back(VertexProperties{
      .x = node_object.at("x").get<double>(),
      .y = node_object.at("y").get<double>(),
    });
  }

  std::vector<std::vector<Graph::Edge>> collated_adjacencies;
  collated_adjacencies.resize(this->vertices_.size());

  const auto& edges = root.at("edges").get<picojson::array>();
  for (const auto& edge_value : edges)
  {
    const auto& edge_object = edge_value.get<picojson::object>();
    const vertex_id_t src_vertex_id = edge_object.at("u").get<double>();
    const vertex_id_t dst_vertex_id = edge_object.at("v").get<double>();
    const edge_weight_t weight = std::max<edge_weight_t>(1, edge_object.at("w").get<double>());
    collated_adjacencies[src_vertex_id].emplace_back(dst_vertex_id, weight);
  }

  this->edges_.reserve(edges.size());
  this->adjacencies_.reserve(nodes.size());
  std::size_t idx = 0;
  for (const auto& e : collated_adjacencies)
  {
    std::size_t idx_start = idx;
    for (const auto& edge : e)
    {
      this->edges_.emplace_back(edge);
      ++idx;
    }
    this->adjacencies_.emplace_back(this->edges_.data() + idx_start, this->edges_.data() + idx);
  }
}

}  // namespace cppcon::demo::vn