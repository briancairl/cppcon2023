#pragma once

// CppCon
#include <cppcon/demo/run.h>

namespace cppcon::demo::v1
{

void run(const std::filesystem::path& graph_in_json, const std::filesystem::path& result_out_json, float percentage_of_problems);

}  // namespace cppcon::demo::v1