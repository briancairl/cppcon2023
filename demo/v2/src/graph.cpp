// CppCon
#include <cppcon/demo/v2/graph.h>
#include <cppcon/demo/json.h>

namespace cppcon::demo::v2
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

    this->adjacencies_.try_emplace(this->vertices_.size());
    this->vertices_.push_back(VertexProperties{
      .x = node_object.at("x").get<double>(),
      .y = node_object.at("y").get<double>(),
    });
  }

  const auto& edges = root.at("edges").get<picojson::array>();
  for (const auto& edge_value : edges)
  {
    const auto& edge_object = edge_value.get<picojson::object>();
    const vertex_id_t pred = edge_object.at("u").get<double>();
    const vertex_id_t succ = edge_object.at("v").get<double>();
    const edge_weight_t weight = std::max<edge_weight_t>(1, edge_object.at("w").get<double>());

    auto itr = this->adjacencies_.find(pred);
    itr->second.emplace_back(succ, weight);
  }
}

}  // namespace cppcon::demo::v2