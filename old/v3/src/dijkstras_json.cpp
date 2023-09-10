#include <algorithm>
#include <fstream>

#include "cppcon/json.h"
#include "cppcon/v3/dijkstras.h"

namespace cppcon::v3
{

Graph Graph::read(const std::filesystem::path& graph_file_name)
{
  Graph g;

  std::ifstream ifs{graph_file_name};
  if (!ifs.is_open())
  {
    throw std::invalid_argument{graph_file_name};
  }

  const auto v = load_json(graph_file_name);
  const auto& root = v.get<picojson::object>();
  const auto& nodes = root.at("nodes").get<picojson::array>();

  g.vertices_.reserve(nodes.size());
  for (const auto& node_value : nodes)
  {
    const auto& node_object = node_value.get<picojson::object>();
    g.adjacencies_[g.vertices_.size()] = {};
    g.vertices_.push_back(VertexProperties{
      .x = node_object.at("x").get<double>(),
      .y = node_object.at("y").get<double>(),
    });
  }

  const auto& edges = root.at("edges").get<picojson::array>();
  for (const auto& edge_value : edges)
  {
    const auto& edge_object = edge_value.get<picojson::object>();
    const vertex_id_t src_vertex_id = edge_object.at("u").get<double>();
    const vertex_id_t dst_vertex_id = edge_object.at("v").get<double>();
    const edge_weight_t weight = std::max<edge_weight_t>(1, edge_object.at("w").get<double>());

    g.adjacencies_[src_vertex_id].emplace_back(dst_vertex_id, weight);
  }

  return g;
}

}  // namespace cppcon::v3
