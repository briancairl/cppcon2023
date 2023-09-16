#pragma once

// C++ Standard Library
#include <filesystem>
#include <vector>

// CppCon
#include <cppcon/dijkstras.h>

namespace cppcon::demo
{

using Path = std::vector<vertex_id_t>;

void save_results(const std::filesystem::path& results_out_json, const std::vector<std::size_t>& shuffle_indices, const std::vector<std::vector<vertex_id_t>>& results);

struct Settings
{
  float percentage_of_problems = 1.f;

  std::size_t shuffle_seed = 0;
};

template<DijkstrasContext C, DijkstrasGraph G, typename WithContext>
void run(const std::filesystem::path& graph_in_json, const std::filesystem::path& result_out_json, const Settings& settings, WithContext with_ctx);

}  // namespace cppcon::demo