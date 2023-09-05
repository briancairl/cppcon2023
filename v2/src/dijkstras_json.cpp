#include <algorithm>
#include <fstream>

#include "cppcon/json.h"
#include "cppcon/v2/dijkstras.h"

namespace cppcon::v2
{

Graph Graph::read(const std::filesystem::path& graph_file_name)
{
  Graph g;

  std::ifstream ifs{graph_file_name};
  if (!ifs.is_open())
  {
    throw std::invalid_argument{graph_file_name};
  }

  picojson::value v;
  std::string err;
  picojson::parse(v, std::istream_iterator<char>{ifs}, std::istream_iterator<char>{}, &err);
  if (!err.empty())
  {
    throw std::runtime_error{std::move(err)};
  }

  const auto& root = v.get<picojson::object>();
  const auto& nodes = root.at("nodes").get<picojson::array>();

  g.vertices_.reserve(nodes.size());
  for (const auto& node_value : nodes)
  {
    const auto& node_object = node_value.get<picojson::object>();
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

    g.adjacencies_.emplace(
      src_vertex_id,
      std::make_pair(dst_vertex_id, weight)
    );
  }

  return g;
}

}  // namespace cppcon::v2
