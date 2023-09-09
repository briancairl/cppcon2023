// C++ Standard Library
#include <chrono>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string_view>

// CPPCon
#include <cppcon/json.h>
#include "cppcon/v0/dijkstras.h"
#include "cppcon/v1/dijkstras.h"
#include "cppcon/v2/dijkstras.h"
#include "cppcon/v3/dijkstras.h"
#include "cppcon/v4/dijkstras.h"
#include "cppcon/v5/dijkstras.h"

using namespace cppcon;

static constexpr std::size_t kRepeats = 1000;

struct Problem
{
  vertex_id_t start_vertex_id;
  vertex_id_t goal_vertex_id;

  [[nodiscard]]
  static Problem load(const picojson::value& problem)
  {
    return Problem{
      .start_vertex_id = static_cast<vertex_id_t>(problem.get<picojson::object>().at("start").get<double>()),
      .goal_vertex_id = static_cast<vertex_id_t>(problem.get<picojson::object>().at("goal").get<double>())
    };
  }
};

Vector<Problem> load_problems(const std::filesystem::path& problems_json)
{
  const auto root = load_json(problems_json);
  const auto& problems_json_arr = root.get<picojson::object>().at("problems").get<picojson::array>();
  Vector<Problem> problems;
  problems.reserve(problems_json_arr.size());
  std::transform(
    problems_json_arr.begin(),
    problems_json_arr.end(),
    std::back_inserter(problems),
    [](const picojson::value& element) -> Problem
    {
      return Problem::load(element);
    });
  return problems;
}

void save_results(const std::filesystem::path& results_json, const Vector<Vector<vertex_id_t>>& results)
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

  std::ofstream ofs{results_json};
  root.serialize(std::ostream_iterator<char>(ofs));
}

template<typename GraphT, typename SearchT>
int run(const std::filesystem::path& graph_json,
        const std::filesystem::path& problems_json,
        const std::filesystem::path& results_json)
{
  const auto graph = GraphT::read(graph_json);
  const auto problems = load_problems(problems_json);

  std::cerr << "Running: " << problems.size() << " problems." << std::endl;

  SearchT opt{graph};
  Vector<Vector<vertex_id_t>> results;
  {
    const auto t_start = std::chrono::high_resolution_clock::now() ;
    for (const auto& [start_vertex_id, goal_vertex_id] : problems)
    {
      std::size_t success = 0;
      for (std::size_t r = 0; r < kRepeats; ++r)
      {
        if (opt.search(graph, start_vertex_id, goal_vertex_id))
        {
          ++success;
        }
      }

      if (success == kRepeats)
      {
        results.emplace_back(opt.get_path(goal_vertex_id));
      }
      else
      {
        results.emplace_back();
      }
    }
    std::cerr << "Time elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t_start).count() << " us" << std::endl;
  }

  save_results(results_json, results);

  return 0;
}

template<typename GraphT, typename SearchT>
int run_all(const std::filesystem::path& graph_json,
            const std::filesystem::path& results_json)
{
  const auto graph = GraphT::read(graph_json);

  std::cerr << "Running all possible problems." << std::endl;

  SearchT opt{graph};
  Vector<Vector<vertex_id_t>> results;
  {
    const auto t_start = std::chrono::high_resolution_clock::now() ;
    for (vertex_id_t start_vertex_id = 0; start_vertex_id < graph.vertex_count(); start_vertex_id += 10)
    {
      for (vertex_id_t goal_vertex_id = 0; goal_vertex_id < graph.vertex_count(); goal_vertex_id += 10)
      {
        if (opt.search(graph, start_vertex_id, goal_vertex_id))
        {
          results.emplace_back(opt.get_path(goal_vertex_id));
        }
        else
        {
          results.emplace_back();
        }
      }
    }
    std::cerr << "Time elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t_start).count() << " us" << std::endl;
  }

  save_results(results_json, results);

  return 0;
}


int main(int argc, char** argv)
{
  if (argc != 5)
  {
    std::cerr << "failure; expected: " << argv[0] << " <version> <in_graph_json> <in_problems_json> <out_results_json>\n" << std::endl;
    return 1;
  }

  if (std::string_view{argv[3]} == "all")
  {
    const std::string_view version{argv[1]};
    const auto* graph_json_path = argv[2];
    const auto* out_results_json = argv[4];

    if (version == "v0")
    {
      return run_all<v0::Graph, v0::Dijkstras>(graph_json_path, out_results_json);
    }
    else if (version == "v1")
    {
      return run_all<v1::Graph, v1::Dijkstras>(graph_json_path, out_results_json);
    }
    else if (version == "v2")
    {
      return run_all<v2::Graph, v2::Dijkstras>(graph_json_path, out_results_json);
    }
    else if (version == "v3")
    {
      return run_all<v3::Graph, v3::Dijkstras>(graph_json_path, out_results_json);
    }
    else if (version == "v4")
    {
      return run_all<v4::Graph, v4::Dijkstras>(graph_json_path, out_results_json);
    }
    else if (version == "v5")
    {
      return run_all<v5::Graph, v5::Dijkstras>(graph_json_path, out_results_json);
    }
    else
    {
      std::cerr << "Invalid version: " << version << std::endl;
    }
  }
  else
  {
    const std::string_view version{argv[1]};
    const auto* graph_json_path = argv[2];
    const auto* in_problems_json = argv[3];
    const auto* out_results_json = argv[4];

    if (version == "v0")
    {
      return run<v0::Graph, v0::Dijkstras>(graph_json_path, in_problems_json, out_results_json);
    }
    else if (version == "v1")
    {
      return run<v1::Graph, v1::Dijkstras>(graph_json_path, in_problems_json, out_results_json);
    }
    else if (version == "v2")
    {
      return run<v2::Graph, v2::Dijkstras>(graph_json_path, in_problems_json, out_results_json);
    }
    else if (version == "v3")
    {
      return run<v3::Graph, v3::Dijkstras>(graph_json_path, in_problems_json, out_results_json);
    }
    else if (version == "v4")
    {
      return run<v4::Graph, v4::Dijkstras>(graph_json_path, in_problems_json, out_results_json);
    }
    else if (version == "v5")
    {
      return run<v5::Graph, v5::Dijkstras>(graph_json_path, in_problems_json, out_results_json);
    }
    else
    {
      std::cerr << "Invalid version: " << version << std::endl;
    }
  }
  return 1;
}