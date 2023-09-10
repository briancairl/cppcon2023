#pragma once

// C++ Standard Library
#include <filesystem>
#include <vector>

// CppCon
#include <cppcon/dijkstras.h>

namespace cppcon::demo
{

using Path = std::vector<vertex_id_t>;

void save_results(const std::filesystem::path& results_out_json, const std::vector<std::vector<vertex_id_t>>& results);

template<DijkstrasContext C, DijkstrasGraph G>
void run(const std::filesystem::path& graph_in_json, const std::filesystem::path& result_out_json, float percentage_of_problems);

}  // namespace cppcon::demo