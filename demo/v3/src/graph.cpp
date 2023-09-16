// C++ Standard Library
#include <algorithm>

// CppCon
#include <cppcon/demo/v3/graph.h>
#include <cppcon/demo/json.h>

namespace cppcon::demo::v3
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

  std::vector<std::vector<Edge>> collated_adjacencies;
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

void Graph::shuffle(const std::vector<std::size_t>& indices)
{
  {
    auto new_vertices = this->vertices_;
    for (std::size_t i = 0; i < new_vertices.size(); ++i)
    {
      new_vertices[indices[i]] = this->vertices_[i];
    }
    new_vertices.swap(this->vertices_);
  }

  {
    std::vector<std::vector<Edge>> collated_adjacencies;
    collated_adjacencies.resize(this->vertices_.size());
    for (vertex_id_t pred = 0; pred < this->adjacencies_.size(); ++pred)
    {
      const auto& edges = this->adjacencies_[pred];
      collated_adjacencies[pred] = std::vector<Edge>{edges.begin(), edges.end()};
    }

    std::vector<std::vector<Edge>> shuffled_adjacencies;
    shuffled_adjacencies.resize(this->vertices_.size());
    for (vertex_id_t pred = 0; pred < this->adjacencies_.size(); ++pred)
    {
      auto& shuffled = shuffled_adjacencies[indices[pred]];
      shuffled.swap(collated_adjacencies[pred]);
      for (auto& [succ, _] : shuffled)
      {
        succ = indices[succ];
      }
    }

    this->edges_.clear();
    this->adjacencies_.clear();
    std::size_t idx = 0;
    for (const auto& e : shuffled_adjacencies)
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
}

}  // namespace cppcon::demo::v3