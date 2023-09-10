#pragma once

// C++ Standard Library
#include <cmath>
#include <filesystem>
#include <vector>
#include <iostream>

// CppCon
#include <cppcon/dijkstras.h>
#include <cppcon/demo/run.h>

namespace cppcon::demo
{

using Path = std::vector<vertex_id_t>;

template<DijkstrasContext C, DijkstrasGraph G>
void run(const std::filesystem::path& graph_in_json, const std::filesystem::path& result_out_json, float percentage_of_problems)
{
  // Load graph from file
  G graph{graph_in_json};

  const std::size_t total_problems = graph.vertex_count() * graph.vertex_count();
  const std::size_t selected_problems = std::max<std::size_t>(1, percentage_of_problems * total_problems);
  const std::size_t step = std::max<std::size_t>(1, 1.f / std::sqrt(percentage_of_problems));

  std::cerr << "Running: " << (100.0f * percentage_of_problems) <<
               "% (" << selected_problems <<
               ") of " << total_problems <<
               " problems [-> " << result_out_json << "]" << std::endl;

  // Contatiner to collect all successful results
  std::vector<Path> results;
  results.reserve(total_problems);

  // Container to store single resultant path
  Path path;
  path.reserve(graph.vertex_count() / 4);

  // Create context that we will reused
  C ctx;

  const auto t_start = std::chrono::high_resolution_clock::now();

  // Iterate over all possible "goal" vertices
  for (vertex_id_t g = 0; g < graph.vertex_count(); g += step)
  {
    ctx.set_goal(g);

    // Iterate over all possible "start" vertices
    for (vertex_id_t s = 0; s < graph.vertex_count(); s += step)
    {
      // Run the search
      if ((s != g) && search(ctx, graph, s))
      {
        // On success, store resulting path
        path.clear();
        get_reverse_path(std::back_inserter(path), ctx, g);
        std::reverse(path.begin(), path.end());
        results.emplace_back(std::move(path));
      }
    }
  }

  const auto t_duration_approx = (std::chrono::high_resolution_clock::now() - t_start);
  const auto t_duration_approx_secs = std::chrono::duration_cast<std::chrono::duration<double>>(t_duration_approx).count();

  std::cerr << "Solved: " << results.size() <<
               " of " << selected_problems <<
               " problems in: " << t_duration_approx_secs <<
               " seconds" << std::endl;

  save_results(result_out_json, results);
}

}  // namespace cppcon::demo