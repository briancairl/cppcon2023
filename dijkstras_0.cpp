#include <algorithm>

#include "dijkstras_0.h"

bool Dijkstras::search(const Graph& graph, const std::size_t src_vertex_id, const std::size_t dst_vertex_id)
{
  visited_.resize(graph.vertices.size());
  std::fill(visited_.begin(), visited_.end(), Transition{});

  transition_queue_.emplace(Transition{
    .dst_vertex_id = src_vertex_id,
    .cost = 0
  });

  while (!transition_queue_.empty())
  {
    const auto prev = transition_queue_.top();

    visited_[prev.dst_vertex_id] = prev;

    if (prev.dst_vertex_id == dst_vertex_id) [[unlikely]]
    {
      return true;
    }

    transition_queue_.pop();

    for (const auto& edge : graph.vertices[prev.dst_vertex_id].edges)
    {
      if (is_unvisited(edge.dst_vertex_id))
      {
        const Transition next_to_visit{
          .dst_vertex_id = edge.dst_vertex_id,
          .cost = edge.props.cost + prev.cost
        };

        transition_queue_.emplace(next_to_visit);
      }
    }
  }

  return false;
}
