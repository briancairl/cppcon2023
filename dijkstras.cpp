#include <algorithm>
#include <fstream>
#include <stdexcept>

#include <picojson/picojson.h>

#include "dijkstras.h"

namespace cppcon
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

Dijkstras::Dijkstras(const Graph& graph)
{
  for (vertex_id_t vertex_id = 0; vertex_id != graph.vertex_count(); ++vertex_id)
  {
    parents_[vertex_id] = vertex_id;
  }
}

bool Dijkstras::search(const Graph& graph, vertex_id_t src_vertex_id, vertex_id_t dst_vertex_id)
{
  queue_.push(Transition{src_vertex_id, src_vertex_id, 0});

  while (!queue_.empty())
  {
    const auto [parent_vertex_id, child_vertex_id, prev_total_weight] = queue_.top();

    queue_.pop();
    
    if (visited_.count(child_vertex_id))
    {
      continue;
    }

    visited_.emplace(child_vertex_id);

    parents_[child_vertex_id] = parent_vertex_id;

    if (child_vertex_id == dst_vertex_id)
    {
      return true;
    }

    graph.for_each_edge(
      child_vertex_id,
      [this, prev_total_weight](vertex_id_t p_id, vertex_id_t c_id, const edge_weight_t& edge_weight)
      {
        if (edge_weight != std::numeric_limits<edge_weight_t>::max())
        {
          queue_.push(Transition{p_id, c_id, edge_weight + prev_total_weight});
        }
      });
  }

  return false;
}

std::vector<vertex_id_t> Dijkstras::get_path(vertex_id_t dst_vertex_id) const
{
  std::vector<vertex_id_t> path;
  path.emplace_back(dst_vertex_id);
  while (true)
  {
    if (const auto parent_id = parents_.at(path.back()); parent_id == path.back())
    {
      break;
    }
    else
    {
      path.push_back(parent_id);
    }
  }
  std::reverse(path.begin(), path.end());
  return path;
}

}  // namespace cppcon
