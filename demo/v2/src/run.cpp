// CppCon
#include <cppcon/demo/run_impl.ipp>
#include <cppcon/demo/v2/run.h>
#include <cppcon/demo/v2/graph.h>
#include <cppcon/demo/v2/context.h>

namespace cppcon::demo::v2
{

void run(const std::filesystem::path& graph_in_json, const std::filesystem::path& result_out_json, const Settings& settings)
{
  ::cppcon::demo::run<TerminateAtGoal, Graph>(graph_in_json, result_out_json, settings);
}

}  // namespace cppcon::demo::v2