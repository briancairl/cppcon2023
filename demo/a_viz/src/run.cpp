// CppCon
#include <cppcon/demo/run_impl.ipp>
#include <cppcon/demo/a_viz/run.h>
#include <cppcon/demo/a_viz/graph.h>
#include <cppcon/demo/a_viz/context.h>

namespace cppcon::demo::a_viz
{

void run(const std::filesystem::path& graph_in_json, const std::filesystem::path& result_out_json, const Settings& settings)
{
  ::cppcon::demo::run<TerminateAtGoal, Graph>(graph_in_json, result_out_json, settings, [](auto& ctx) { ctx.keep_visited(); });
}

}  // namespace cppcon::demo::a_viz