#pragma once

// C++ Standard Library
#include <type_traits>
#include <limits>
#include <utility>

namespace cppcon
{

using vertex_id_t = std::uint32_t;

using edge_weight_t = std::uint32_t;

struct VertexProperties { double x; double y; /* ... */ };

struct EdgeProperties
{
  bool valid;
  edge_weight_t weight;
  constexpr explicit EdgeProperties(edge_weight_t w) : valid{true}, weight{w} {}
};

using Edge = std::pair<vertex_id_t, EdgeProperties>;

struct Transition
{
  vertex_id_t pred;
  vertex_id_t succ;
  edge_weight_t weight;
};

constexpr bool operator>(const Transition& lhs, const Transition& rhs) { return lhs.weight > rhs.weight; }


template <typename T>
concept SearchGraph = 
  requires(T&& g)
  {
      { g.for_each_edge(vertex_id_t{}, [](vertex_id_t, const EdgeProperties&) {}) };
      { g.vertex_count() };
      { g.vertex(vertex_id_t{}) };
  };


template <typename T>
concept SearchContext = 
  requires(T&& ctx)
  {
      { ctx.is_queue_not_empty() };
      { ctx.is_visited(vertex_id_t{}) };
      { ctx.is_terminal(vertex_id_t{}) };
      { ctx.mark_visited(vertex_id_t{}, vertex_id_t{}) };
      { ctx.enqueue(vertex_id_t{}, vertex_id_t{}, edge_weight_t{}) };
      { ctx.dequeue() };
      { ctx.predecessor(vertex_id_t{}) };
  };



template<SearchContext C, SearchGraph G>
bool search(C& ctx, const G& graph, vertex_id_t start)
{
  ctx.reset(graph, start);

  while (ctx.is_queue_not_empty())
  {
    // De-queue successor vertex with the next smallest total weight
    const auto [pred, succ, total_weight] = ctx.dequeue();

    // Skip successor if it has been visited
    if (ctx.is_visited(succ))
    {
      continue;
    }
    else
    {
      // Mark this successor vertex as visited
      ctx.mark_visited(pred, succ);
    }

    // Stop the search if successor is a terminal state
    if (ctx.is_terminal(succ))
    {
      return true;
    }
    else
    {
      // Iterate over all edges from 'succ'
      graph.for_each_edge(
        succ,
        [&ctx, total_weight, parent=succ](vertex_id_t child, const EdgeProperties& edge) mutable
        {
          if (!edge.valid or ctx.is_visited(child))
          {
            return;
          }
          else
          {
            ctx.enqueue(parent, child, edge.weight + total_weight);
          }
        });
    }
  }

  // Terminal condition not met
  return false;
}


template<typename OutputIteratorT, SearchContext C>
OutputIteratorT get_reverse_path(OutputIteratorT out, const C& ctx, vertex_id_t succ)
{
  (*out) = succ;
  while (true)
  {
    if (const auto pred = ctx.predecessor(succ); pred == succ)
    {
      break;
    }
    else
    {
      (*out) = pred;
      succ = pred;
    }
  }
  return out;
}


}  // namespace cppcon