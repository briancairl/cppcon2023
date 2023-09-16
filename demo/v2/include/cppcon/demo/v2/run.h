#pragma once

// CppCon
#include <cppcon/demo/run.h>

namespace cppcon::demo::v2
{

void run(const std::filesystem::path& graph_in_json, const std::filesystem::path& result_out_json, const Settings& settings);

}  // namespace cppcon::demo::v2