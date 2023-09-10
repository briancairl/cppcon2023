#pragma once

// C++ Standard Library
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

// CppCon
#include <cppcon/dijkstras.h>

namespace cppcon::demo::v2
{

class TerminateAtGoal
{
public:
  void set_goal(vertex_id_t g) { goal_ = g; }

  template<DijkstrasGraph G>
  void reset([[maybe_unused]] G&& graph, vertex_id_t s)
  {
    visited_.clear();
    while (!queue_.empty()) { queue_.pop(); }
    enqueue(s, s, 0);
  }

  bool is_searching() const { return !queue_.empty(); }

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

  std::unordered_map<vertex_id_t, vertex_id_t> visited_;
};


}  // namespace cppcon::demo::v2