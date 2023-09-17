#pragma once

// C++ Standard Library
#include <algorithm>
#include <filesystem>
#include <queue>
#include <map>
#include <utility>
#include <vector>

// CppCon
#include <cppcon/search.h>

namespace cppcon::demo::viz
{

template<typename T>
struct MinQueue : std::priority_queue<T, std::vector<T>, std::greater<T>>
{
  using Base = std::priority_queue<T, std::vector<T>, std::greater<T>>;
  using Base::Base;
  std::vector<T>& underlying() { return Base::c; }
};

void save_visited(const std::filesystem::path& results_out_json, const std::vector<std::vector<vertex_id_t>>& results);

class TerminateAtGoal
{
public:
  ~TerminateAtGoal()
  {
    save_visited(result_file_name, visited_results_);
  }

  void keep_visited() { visited_results_.push_back(visited_order_); }

  void set_goal(vertex_id_t g) { goal_ = g; }

  template<SearchGraph G>
  void reset(G&& graph, vertex_id_t s)
  {
    if (result_file_name.empty())
    {
      result_file_name = graph.graph_file_name;
      result_file_name.replace_extension(".visited_order.json");
    }

    queue_back_buffer_.clear();
    queue_.underlying().swap(queue_back_buffer_);

    visited_order_.clear();
    visited_order_.reserve(graph.vertex_count());

    visited_.resize(graph.vertex_count());
    visited_.assign(graph.vertex_count(), graph.vertex_count());

    enqueue(s, s, 0);
  }

  bool is_queue_not_empty() const { return !queue_.empty(); }

  bool is_visited(vertex_id_t q) const { return visited_[q] != visited_.size(); }

  bool is_terminal(vertex_id_t q) const { return goal_ == q; }

  void mark_visited(vertex_id_t p, vertex_id_t s)
  {
    visited_[s] = p;
    visited_order_.push_back(p);
  }

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

  std::filesystem::path result_file_name;

private:
  vertex_id_t goal_;

  MinQueue<Transition> queue_;
  std::vector<Transition> queue_back_buffer_;

  std::vector<vertex_id_t> visited_;
  std::vector<vertex_id_t> visited_order_;
  std::vector<std::vector<vertex_id_t>> visited_results_;
};


}  // namespace cppcon::demo::viz