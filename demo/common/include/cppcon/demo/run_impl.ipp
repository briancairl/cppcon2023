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
void run(const std::filesystem::path& graph_in_json, const std::filesystem::path& result_out_json, const Settings& settings)
{
  // Load graph from file
  G graph{graph_in_json};

  const std::size_t total_problems = graph.vertex_count() * graph.vertex_count();
  const std::size_t selected_problems = std::max<std::size_t>(1, settings.percentage_of_problems * total_problems);
  const std::size_t step = std::max<std::size_t>(1, 1.f / std::sqrt(settings.percentage_of_problems));

  // Contatiner to collect all successful results
  std::vector<Path> results;
  results.reserve(total_problems);

  // Container to store single resultant path
  Path path;
  path.reserve(graph.vertex_count() / 4);

  // Create context that we will reused
  C ctx;

  std::vector<std::size_t> shuffle_indices;

  const auto Update = [&shuffle_indices, original=graph, n=settings.shuffles](G& graph, int shuffle_index) -> bool
  {
    if (shuffle_index > n)
    {
      return false;
    }
    else if (shuffle_index == 0)
    {
      return true;
    }
    else if (shuffle_index == n)
    {
      graph = original;
      shuffle_indices.clear();
    }
    else
    {
      shuffle_indices = graph.shuffle();
    }
    return true;
  };

  for (int shuffle_index = 0; Update(graph, shuffle_index); ++shuffle_index)
  {
    std::cerr << "SHUFFLE[" << shuffle_index << "] -- " <<
                 "Running: " << (100.0f * settings.percentage_of_problems) <<
                 "% (" << selected_problems <<
                 ") of " << total_problems <<
                 " problems" << std::endl;
    results.clear();

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

    std::filesystem::path shuffle_path{"." + std::to_string(shuffle_index) + result_out_json.extension().string()};
    shuffle_path = std::filesystem::path{result_out_json}.replace_extension(shuffle_path);

    std::cerr << "SHUFFLE[" << shuffle_index << "] -- " <<
                 "Solved: " << results.size() <<
                 " of " << selected_problems <<
                 " problems in: " << t_duration_approx_secs <<
                 " seconds --> " << shuffle_path << std::endl;
    save_results(shuffle_path, shuffle_indices, results);
  }
}

}  // namespace cppcon::demo