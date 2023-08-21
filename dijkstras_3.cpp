#include <algorithm>

#include "dijkstras_3.h"

Graph::Graph(std::uint32_t vertex_count, std::uint32_t edge_count)
{
  vertices.resize(vertex_count);
  adjacencies.resize(vertex_count);
  for (auto& adj : adjacencies)
  {
    adj.reserve(edge_count);
  }
}

bool Graph::has_edge(std::uint32_t src_vertex_id, std::uint32_t dst_vertex_id) const
{
  const auto& c = adjacencies[src_vertex_id];
  return std::any_of(c.begin(), c.end(), [dst_vertex_id](const Adjacency& adj) { return adj.dst_vertex_id == dst_vertex_id; });
}

void Graph::add_edge(std::uint32_t src_vertex_id, std::uint32_t dst_vertex_id, const EdgeProperties& edge_props)
{
  const auto edge_id = edges.size();
  adjacencies[src_vertex_id].push_back(Adjacency{
    .edge_id = edge_id,
    .dst_vertex_id = dst_vertex_id,
    .cost = edge_props.cost
  });
  edges.push_back(edge_props);
}

bool Dijkstras::search(const Graph& graph, const std::uint32_t src_vertex_id, const std::uint32_t dst_vertex_id)
{
  visited_.resize(graph.vertices.size());
  transitions_.resize(graph.vertices.size());
  std::fill(visited_.begin(), visited_.end(), false);

  transition_queue_.reserve(graph.vertices.size());
  transition_queue_.push_back(Transition{src_vertex_id, 0});

  just_visited_.reserve(10);

  while (!transition_queue_.empty())
  {
    const auto prev = transition_queue_.back();

    visited_[prev.dst_vertex_id] = true;
    transitions_[prev.dst_vertex_id] = prev.dst_vertex_id;

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
        transition_queue_.push_back(Transition{edge.dst_vertex_id, edge.cost + prev.cost});
        std::push_heap(
        transition_queue_.begin(),
        transition_queue_.end(),
        std::greater<Transition>{}
      );
      }
    }
  }

  return false;
}
