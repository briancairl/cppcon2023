// CppCon
#include <cppcon/demo/run_impl.ipp>
#include <cppcon/demo/v3/run.h>
#include <cppcon/demo/v3/graph.h>
#include <cppcon/demo/v3/context.h>

namespace cppcon::demo::v3
{

void run(const std::filesystem::path& graph_in_json, const std::filesystem::path& result_out_json, const Settings& settings)
{
  ::cppcon::demo::run<TerminateAtGoal, Graph>(graph_in_json, result_out_json, settings, []([[maybe_unused]] auto& ctx) {});
}

}  // namespace cppcon::demo::v3
