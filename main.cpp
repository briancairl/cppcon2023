#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <functional>
#include <limits>
#include <vector>
#include <queue>

#include "arena_allocator.h"

template<typename T>
using Alloc =
#ifdef USE_ARENA
  arena_allocator<T>
#else
  std::allocator<T>
#endif
;

template<typename T>
using Vector = std::vector<T, Alloc<T>>;

struct Adjacency
{
  std::size_t edge_id = 0;
  std::size_t dst_vertex_id = 0;
};

struct VertexProperties
{
  float x = 0.f;
  float y = 0.f;
};

struct EdgeProperties
{
  std::size_t cost = 1;
};

struct Graph
{
  Vector<Vector<Adjacency>> adjacencies;
  Vector<VertexProperties> vertices;
  Vector<EdgeProperties> edges;

  explicit Graph(std::size_t vertex_count)
  {
    vertices.resize(vertex_count);
    adjacencies.resize(vertex_count);
  }

  bool has_edge(std::size_t src_vertex_id, std::size_t dst_vertex_id) const
  {
    const auto& c = adjacencies[src_vertex_id];
    return std::any_of(c.begin(), c.end(), [dst_vertex_id](const Adjacency& adj) { return adj.dst_vertex_id == dst_vertex_id; });
  }

  void add_edge(std::size_t src_vertex_id, std::size_t dst_vertex_id, const EdgeProperties& edge_props)
  {
    const auto edge_id = edges.size();
    adjacencies[src_vertex_id].emplace_back(Adjacency{
      .edge_id = edge_id,
      .dst_vertex_id = dst_vertex_id
    });
    edges.emplace_back(edge_props);
  }
};

struct Transition
{
  std::size_t src_vertex_id = 0;
  std::size_t dst_vertex_id = 0;
  std::size_t cost = std::numeric_limits<std::size_t>::max();
};

bool operator>(const Transition& lhs, const Transition& rhs)
{
  return lhs.cost > rhs.cost;
}

class Dijkstras
{
public:
  bool is_unvisited(std::size_t vertex_id) const
  {
    return visited_[vertex_id].cost == std::numeric_limits<std::size_t>::max();
  }

  bool search(const Graph& graph, const std::size_t src_vertex_id, const std::size_t dst_vertex_id)
  {
    visited_.resize(graph.vertices.size(), Transition{});

    transition_queue_.emplace(Transition{
      .src_vertex_id = src_vertex_id,
      .dst_vertex_id = src_vertex_id,
      .cost = 0
    });

    while (!transition_queue_.empty())
    {
      [[maybe_unused]] const auto prev = transition_queue_.top();

      visited_[prev.dst_vertex_id] = prev;

      if (prev.dst_vertex_id == dst_vertex_id)
      {
        return true;
      }

      transition_queue_.pop();

      for (const auto& edge : graph.adjacencies[prev.dst_vertex_id])
      {
        if (is_unvisited(edge.dst_vertex_id))
        {
          const Transition next_transition{
            .src_vertex_id = prev.dst_vertex_id,
            .dst_vertex_id = edge.dst_vertex_id,
            .cost = graph.edges[edge.edge_id].cost + prev.cost
          };

          transition_queue_.emplace(next_transition);
        }
      }
    }

    return false;
  }

private:
  Vector<Transition> visited_;
  std::priority_queue<Transition, Vector<Transition>, std::greater<Transition>> transition_queue_;
};

int main(int argc, char** argv)
{
  const std::size_t kVertexCount = 1234;

  Graph graph{kVertexCount};

  {
    const auto t_start = std::chrono::steady_clock::now();
    for (std::size_t src_vertex_id = 0; src_vertex_id < kVertexCount; ++src_vertex_id)
    {
      const std::size_t n = std::rand() % 50;
      for (std::size_t i = 0; i < n; ++i)
      {
        const std::size_t dst_vertex_id = std::rand() % kVertexCount;
        if (!graph.has_edge(src_vertex_id, dst_vertex_id))
        {
          graph.add_edge(src_vertex_id, dst_vertex_id, EdgeProperties{1});
        }
      }
    }
  #ifdef USE_ARENA
    std::fprintf(stderr, "USE_ARENA:DT =  %lu us\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_start).count());
  #else
    std::fprintf(stderr, "DT =  %lu us\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_start).count());
  #endif
  }

  int n = 0;
  {
    const auto t_start = std::chrono::steady_clock::now();
    Dijkstras d;
    for (std::size_t src_vertex_id = 0; src_vertex_id < kVertexCount; ++src_vertex_id)
    {
      for (std::size_t dst_vertex_id = src_vertex_id+1; dst_vertex_id < kVertexCount; ++dst_vertex_id)
      {
        n += d.search(graph, src_vertex_id, dst_vertex_id);
        n += d.search(graph, dst_vertex_id, src_vertex_id);
      }
    }

  #ifdef USE_ARENA
    std::fprintf(stderr, "USE_ARENA:DT =  %lu us\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_start).count());
  #else
    std::fprintf(stderr, "DT =  %lu us\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_start).count());
  #endif
  }

  return n > 0 ? 0 : 1;
}