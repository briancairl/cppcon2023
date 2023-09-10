#include <algorithm>
#include <stdexcept>

#include "cppcon/v4/dijkstras.h"

namespace cppcon::v4
{
template<typename EdgeVisitorT>
void Graph::for_each_edge(vertex_id_t parent_vertex_id, EdgeVisitorT visitor) const
{
  std::for_each(
    adjacencies_[parent_vertex_id].begin(),
    adjacencies_[parent_vertex_id].end(),
    [parent_vertex_id, visitor](const auto& child_and_edge_weight)
    {
      const auto& [child_vertex_id, edge_weight] = child_and_edge_weight;
      visitor(parent_vertex_id, child_vertex_id, edge_weight);
    });
}

Dijkstras::Dijkstras(const Graph& graph)
{
  queue_back_buffer_.reserve(graph.vertex_count());
}

bool Dijkstras::search(const Graph& graph, const vertex_id_t start_vertex_id, const vertex_id_t goal_vertex_id)
{
  visited_.clear();

  queue_back_buffer_.clear();
  queue_back_buffer_.swap(queue_.underlying());

  queue_.push(Transition{start_vertex_id, start_vertex_id, 0});

  while (!queue_.empty())
  {
    const auto [parent_vertex_id, child_vertex_id, prev_total_weight] = queue_.top();

    queue_.pop();

    if (is_visited(child_vertex_id))
    {
      continue;
    }

    visited_[child_vertex_id] = parent_vertex_id;

    if (child_vertex_id == goal_vertex_id)
    {
      return true;
    }

    graph.for_each_edge(
      child_vertex_id,
      [this, prev_total_weight](const vertex_id_t p_id, const vertex_id_t c_id, const edge_weight_t edge_weight)
      {
        if (edge_weight != std::numeric_limits<edge_weight_t>::max() && !is_visited(c_id))
        {
          queue_.push(Transition{p_id, c_id, edge_weight + prev_total_weight});
        }
      });
  }

  return false;
}

Vector<vertex_id_t> Dijkstras::get_path(vertex_id_t goal_vertex_id) const
{
  Vector<vertex_id_t> path;
  path.emplace_back(goal_vertex_id);
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

}  // namespace cppcon::v4
