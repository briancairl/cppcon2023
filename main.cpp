#include <cctype>
#include <cstdio>
#include <functional>
#include <limits>
#include <vector>
#include <queue>

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
  std::vector<std::vector<Adjacency>> adjacencies;
  std::vector<VertexProperties> vertices;
  std::vector<EdgeProperties> edges;

  explicit Graph(std::size_t vertex_count)
  {
    vertices.resize(vertex_count);
    adjacencies.resize(vertex_count);
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
  std::vector<Transition> visited_;
  std::priority_queue<Transition, std::vector<Transition>, std::greater<Transition>> transition_queue_;
};

int main(int argc, char** argv)
{
  const std::size_t kVertexCount = 123123;

  Graph graph{kVertexCount};
  for (std::size_t v = 1; v < kVertexCount; ++v)
  {
    graph.add_edge(v-1, v, EdgeProperties{1});
  }

  Dijkstras d;

  if (d.search(graph, 0, 10))
  {
    std::fprintf(stderr, "%s\n", "found path to goal");
  }
  else
  {
    std::fprintf(stderr, "%s\n", "failed"); 
  }

  return 0;
}