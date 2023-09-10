// CppCon
#include <cppcon/demo/run_impl.ipp>
#include <cppcon/demo/v1/run.h>
#include <cppcon/demo/v1/graph.h>
#include <cppcon/demo/v1/context.h>

namespace cppcon::demo::v1
{

void run(const std::filesystem::path& graph_in_json, const std::filesystem::path& result_out_json, float percentage_of_problems)
{
  ::cppcon::demo::run<TerminateAtGoal, Graph>(graph_in_json, result_out_json, percentage_of_problems);
}

}  // namespace cppcon::demo::v1