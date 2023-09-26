// C++ Standard Library
#include <algorithm>

// CppCon
#include <cppcon/demo/v0/graph.h>
#include <cppcon/demo/json.h>

namespace cppcon::demo::v0
{

Graph::Graph(const std::filesystem::path& graph_file_name)
{
  const auto v = load_json(graph_file_name);
  const auto& root = v.get<picojson::object>();
  const auto& nodes = root.at("nodes").get<picojson::array>();

  for (const auto& node_value : nodes)
  {
    const auto& node_object = node_value.get<picojson::object>();
    this->vertices_.emplace(
      this->vertices_.size(),
      VertexProperties{
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

    this->adjacencies_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(pred),
      std::forward_as_tuple(succ, weight)
    );
  }
}

void Graph::shuffle(const std::vector<std::size_t>& indices)
{
  {
    using VerticesType = decltype(this->vertices_);
    VerticesType new_vertices;
    for (const auto& [i, prop] : this->vertices_)
    {
      new_vertices[indices[i]] = prop;
    }
    new_vertices.swap(this->vertices_);
  }

  {
    using AdjacenciesType = decltype(this->adjacencies_);
    AdjacenciesType new_adjacencies;
    for (const auto& [pred, edge] : this->adjacencies_)
    {
      const auto& [succ, edge_props] = edge;
      new_adjacencies.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(indices[pred]),
        std::forward_as_tuple(indices[succ], edge_props)
      );
    }
    new_adjacencies.swap(this->adjacencies_);
  }
}

}  // namespace cppcon::demo::v0