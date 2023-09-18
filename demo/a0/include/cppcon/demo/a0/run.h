#pragma once

// CppCon
#include <cppcon/demo/run.h>

namespace cppcon::demo::a0
{

void run(const std::filesystem::path& graph_in_json, const std::filesystem::path& result_out_json, const Settings& settings);

}  // namespace cppcon::demo::a0