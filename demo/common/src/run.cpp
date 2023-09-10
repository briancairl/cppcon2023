// C++ Standard Library
#include <algorithm>
#include <fstream>

// CppCon
#include <cppcon/demo/run.h>
#include <cppcon/demo/json.h>

namespace cppcon::demo
{

void save_results(const std::filesystem::path& results_out_json, const std::vector<std::vector<vertex_id_t>>& results)
{
  picojson::array results_array;
  results_array.reserve(results.size());
  for (const auto& path : results)
  {
    results_array.emplace_back(
      [&path]() -> picojson::value
      {
        picojson::array arr_json;
        arr_json.reserve(path.size());
        std::transform(
          path.begin(),
          path.end(),
          std::back_inserter(arr_json),
          [](const vertex_id_t vid) -> picojson::value
          {
            return picojson::value{static_cast<double>(vid)};
          });
        picojson::object path_object;
        path_object["path"] = picojson::value{std::move(arr_json)};
        return picojson::value{std::move(path_object)};
      }());
  }

  picojson::object root_object;
  root_object["results"] = picojson::value{std::move(results_array)};
  picojson::value root{std::move(root_object)};

  std::ofstream ofs{results_out_json};
  root.serialize(std::ostream_iterator<char>(ofs));
}

}  // namespace cppcon::demo