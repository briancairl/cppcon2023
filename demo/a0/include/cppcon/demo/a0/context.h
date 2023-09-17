#pragma once

// C++ Standard Library
#include <algorithm>
#include <cmath>
#include <queue>
#include <map>
#include <utility>
#include <vector>

// CppCon
#include <cppcon/search.h>

namespace cppcon::demo::a0
{

class TerminateAtGoal
{
public:
  void set_goal(vertex_id_t g) { goal_ = g; }

  template<SearchGraph G>
  void reset([[maybe_unused]] G&& graph, vertex_id_t s)
  {
    visited_.clear();
    while (!queue_.empty()) { queue_.pop(); }

    heuristic_.resize(graph.vertex_count());
    {
      const auto& vg = graph.vertex(goal_);
      for (vertex_id_t i = 0; i < graph.vertex_count(); ++i)
      {
        const auto& vq = graph.vertex(i);
        const double dx = (vg.x - vq.x);
        const double dy = (vg.y - vq.y);
        heuristic_[i] = std::sqrt(dx * dx + dy * dy);
      }
    }

    enqueue(s, s, 0);
  }

  bool is_queue_not_empty() const { return !queue_.empty(); }

  bool is_visited(vertex_id_t q) const { return visited_.count(q); }

  bool is_terminal(vertex_id_t q) const { return goal_ == q; }

  void mark_visited(vertex_id_t p, vertex_id_t s) { visited_.emplace(s, p); }

  vertex_id_t predecessor(vertex_id_t q) const
  {
    if (const auto itr = visited_.find(q); itr == visited_.end())
    {
      return q;
    }
    else
    {
      return itr->second;
    }
  }

  Transition dequeue()
  {
    auto t = queue_.top();
    queue_.pop();
    return t;
  }

  void enqueue(vertex_id_t p, vertex_id_t s, edge_weight_t w)
  {
    queue_.push(Transition{
      .pred = p,
      .succ = s,
      .weight = w
    });
  }

private:
  vertex_id_t goal_;

  std::priority_queue<Transition, std::vector<Transition>, std::greater<Transition>> queue_;

  std::map<vertex_id_t, vertex_id_t> visited_;

  std::vector<edge_weight_t> heuristic_;
};


}  // namespace cppcon::demo::a0