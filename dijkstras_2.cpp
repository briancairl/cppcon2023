#include <algorithm>

#include "dijkstras_2.h"

bool Dijkstras::search(const Graph& graph, const std::size_t src_vertex_id, const std::size_t dst_vertex_id)
{
  visited_.resize(graph.vertices.size());
  std::fill(visited_.begin(), visited_.end(), Transition{});

  transition_queue_.emplace_back(Transition{
    .dst_vertex_id = src_vertex_id,
    .cost = 0
  });

  just_visited_.reserve(2 * graph.edges.size() / graph.vertices.size());

  while (!transition_queue_.empty())
  {
    const auto prev = transition_queue_.back();

    visited_[prev.dst_vertex_id] = prev;

    if (prev.dst_vertex_id == dst_vertex_id) [[unlikely]]
    {
      return true;
    }

    std::pop_heap(
      transition_queue_.begin(),
      transition_queue_.end(),
      std::greater<Transition>{}
    );
    transition_queue_.pop_back();

    for (const auto& edge : graph.adjacencies[prev.dst_vertex_id])
    {
      if (is_unvisited(edge.dst_vertex_id))
      {
        const Transition next_to_visit{
          .dst_vertex_id = edge.dst_vertex_id,
          .cost = graph.edges[edge.edge_id].cost + prev.cost
        };

        transition_queue_.emplace_back(next_to_visit);
      }
    }

    for (const auto& v : just_visited_)
    {
      transition_queue_.emplace_back(v);
      std::push_heap(
        transition_queue_.begin(),
        transition_queue_.end(),
        std::greater<Transition>{}
      );
    }

    just_visited_.clear();
  }

  return false;
}
