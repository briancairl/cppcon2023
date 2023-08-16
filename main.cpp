#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <functional>
#include <limits>
#include <vector>
#include <queue>

#include "graph.h"
#include "dijkstras.h"

#include <sched.h>

int main(int argc, char** argv)
{
  cpu_set_t  mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  if (const int result = sched_setaffinity(0, sizeof(mask), &mask); result != 0)
  {
    return 1;
  }


  const std::size_t kVertexCount = 123;
  const std::size_t kEdgeCount = 5;

  Graph graph{kVertexCount, kEdgeCount};

  {
    const auto t_start = std::chrono::steady_clock::now();
    for (std::size_t src_vertex_id = 0; src_vertex_id < kVertexCount; ++src_vertex_id)
    {
      const std::size_t n = std::rand() % kEdgeCount;
      for (std::size_t i = 0; i < n; ++i)
      {
        const std::size_t dst_vertex_id = std::rand() % kVertexCount;
        if (!graph.has_edge(src_vertex_id, dst_vertex_id))
        {
          graph.add_edge(src_vertex_id, dst_vertex_id, EdgeProperties{1});
        }
      }
    }
    std::fprintf(stderr, "CREATE DT =  %lu us\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_start).count());
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
    std::fprintf(stderr, "SEARCH DT =  %lu us\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_start).count());
  }

  return n > 0 ? 0 : 1;
}