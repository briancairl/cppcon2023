#include <algorithm>
#include <stdexcept>

#include "cppcon/v1/dijkstras.h"

namespace cppcon::v1
{

template<typename EdgeVisitorT>
void Graph::for_each_edge(vertex_id_t parent_vertex_id, EdgeVisitorT visitor) const
{
  auto range = adjacencies_.equal_range(parent_vertex_id);
  std::for_each(
    range.first,
    range.second,
    [visitor](const auto& parent_and_edge)
    {
      const auto& [parent_vertex_id, child_vertex_id_and_edge_props] = parent_and_edge;
      const auto& [child_vertex_id, edge_properties] = child_vertex_id_and_edge_props;
      visitor(parent_vertex_id, child_vertex_id, edge_properties);
    });
}

Dijkstras::Dijkstras([[maybe_unused]] const Graph& graph) {}

bool Dijkstras::search(const Graph& graph, vertex_id_t src_vertex_id, vertex_id_t dst_vertex_id)
{
  visited_.clear();

  while (!queue_.empty()) { queue_.pop(); }

  queue_.push(Transition{src_vertex_id, src_vertex_id, 0});
  while (!queue_.empty())
  {
    const auto [parent_vertex_id, child_vertex_id, prev_total_weight] = queue_.top();

    queue_.pop();
    
    if (const auto [_, is_new] = visited_.try_emplace(child_vertex_id, parent_vertex_id); !is_new)
    {
      continue;
    }
    else if (child_vertex_id == dst_vertex_id)
    {
      return true;
    }

    graph.for_each_edge(
      child_vertex_id,
      [this, prev_total_weight](const vertex_id_t p_id, const vertex_id_t c_id, const edge_weight_t edge_weight)
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
    if (const auto parent_id = visited_.at(path.back()); parent_id == path.back())
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

}  // namespace cppcon::v1
