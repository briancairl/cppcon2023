#pragma once

// C++ Standard Library
#include <algorithm>
#include <queue>
#include <map>
#include <utility>
#include <vector>

// CppCon
#include <cppcon/dijkstras.h>

namespace cppcon::demo::vn
{

template<typename T>
struct MinQueue : std::priority_queue<T, std::vector<T>, std::greater<T>>
{
  using Base = std::priority_queue<T, std::vector<T>, std::greater<T>>;
  using Base::Base;
  std::vector<T>& underlying() { return Base::c; }
};

class TerminateAtGoal
{
public:
  void set_goal(vertex_id_t g) { goal_ = g; }

  template<DijkstrasGraph G>
  void reset(G&& graph, vertex_id_t s)
  {
    queue_back_buffer_.clear();
    queue_.underlying().swap(queue_back_buffer_);

    visited_.resize(graph.vertex_count());
    visited_.assign(graph.vertex_count(), graph.vertex_count());

    enqueue(s, s, 0);
  }

  bool is_queue_not_empty() const { return !queue_.empty(); }

  bool is_visited(vertex_id_t q) const { return visited_[q] != visited_.size(); }

  bool is_terminal(vertex_id_t q) const { return goal_ == q; }

  void mark_visited(vertex_id_t p, vertex_id_t s) { visited_[s] = p; }

  vertex_id_t predecessor(vertex_id_t q) const
  {
    return visited_[q];
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

  MinQueue<Transition> queue_;
  std::vector<Transition> queue_back_buffer_;

  std::vector<vertex_id_t> visited_;
};


}  // namespace cppcon::demo::vn