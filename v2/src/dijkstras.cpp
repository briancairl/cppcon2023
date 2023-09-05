#include <algorithm>
#include <stdexcept>

#include "cppcon/v2/dijkstras.h"

namespace cppcon::v2
{

namespace {

template<typename T, typename IteratorT>
void push(Vector<T>& heap, IteratorT last)
{
  std::push_heap(heap.begin(), last, std::greater<T>{});
}

template<typename T>
void pop(Vector<T>& heap)
{
  std::pop_heap(heap.begin(), heap.end(), std::greater<T>{});
}

}  // namespace

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

bool Dijkstras::search(const Graph& graph, const vertex_id_t start_vertex_id, const vertex_id_t goal_vertex_id)
{
  visited_.clear();

  queue_.clear();
  queue_.push_back(Transition{start_vertex_id, start_vertex_id, 0});
  push(queue_, queue_.end());

  while (!queue_.empty())
  {
    pop(queue_);

    const auto [parent_vertex_id, child_vertex_id, prev_total_weight] = queue_.back();

    queue_.pop_back();

    if (const auto [_, is_new] = visited_.try_emplace(child_vertex_id, parent_vertex_id); !is_new)
    {
      continue;
    }
    else if (child_vertex_id == goal_vertex_id)
    {
      return true;
    }

    const std::size_t previous_heap_size = queue_.size();

    graph.for_each_edge(
      child_vertex_id,
      [this, prev_total_weight](const vertex_id_t p_id, const vertex_id_t c_id, const edge_weight_t edge_weight)
      {
        if (edge_weight != std::numeric_limits<edge_weight_t>::max())
        {
          queue_.push_back(Transition{p_id, c_id, edge_weight + prev_total_weight});
        }
      });

    auto last_itr = std::next(queue_.begin(), previous_heap_size);
    while (last_itr != queue_.end())
    {
      ++last_itr;
      push(queue_, last_itr);
    }
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

}  // namespace cppcon::v2
