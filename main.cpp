// C++ Standard Library
#include <chrono>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string_view>

// Unix
#include <sched.h>

// PicoJSON
#include <picojson/picojson.h>

// CPPCon
#include "cppcon/v0/dijkstras.h"
#include "cppcon/v1/dijkstras.h"
#include "cppcon/v2/dijkstras.h"
#include "cppcon/v3/dijkstras.h"
#include "cppcon/v4/dijkstras.h"

using namespace cppcon;


void dump_path(const std::filesystem::path& path_file_name, const Vector<vertex_id_t>& path)
{
  std::ofstream ofs{path_file_name};

  picojson::value path_array_json{picojson::array{}};
  {
    auto& path_array = path_array_json.get<picojson::array>();
    for (const vertex_id_t vid : path)
    {
      path_array.push_back(picojson::value{static_cast<double>(vid)});
    }
  }

  picojson::value root{picojson::object{}};
  {
    auto& root_object = root.get<picojson::object>();
    root_object["path"] = std::move(path_array_json);
  }
  root.serialize(std::ostream_iterator<char>(ofs));
}

template<typename T>
T to(const char* const value_str)
{
  T value;
  std::stringstream ss;
  ss << value_str;
  ss >> value;
  return value;
}

template<typename GraphT, typename SearchT>
int run(const char* label,
        const std::filesystem::path& graph_json,
        vertex_id_t start_vertex_id,
        vertex_id_t goal_vertex_id,
        int iterations)
{
  const auto graph = GraphT::read(graph_json);

  SearchT opt{graph};

  const auto t_start = std::chrono::high_resolution_clock::now() ;

  for (int i = 0; i < iterations; ++i)
  {
    if (!opt.search(graph, start_vertex_id, goal_vertex_id))
    {
      return 1;
    }
  }

  {
    std::ostringstream oss;
    oss << label << "_" << graph_json.stem().string() << "_" << start_vertex_id << "_" << goal_vertex_id << ".json";
    dump_path(oss.str(), opt.get_path(goal_vertex_id));
    std::cerr << oss.str() << std::endl;
  }

  std::cerr << "Time elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t_start).count() << " us" << std::endl;

  return 0;
}


int main(int argc, char** argv)
{
  if (argc != 6)
  {
    std::fprintf(stderr, "failure; expected: %s <version> <in_graph_json> <start_vertex_id> <goal_vertex_id> <search count>\n", argv[0]);
    return 1;
  }

  cpu_set_t  mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  if (const int result = sched_setaffinity(0, sizeof(mask), &mask); result != 0)
  {
    return 1;
  }

  const std::string_view version{argv[1]};
  const auto* graph_json_path = argv[2];
  const auto start_vertex_id = to<vertex_id_t>(argv[3]);
  const auto goal_vertex_id = to<vertex_id_t>(argv[4]);
  const auto iterations = to<int>(argv[5]);

  int retval = 1;
  if (version == "v0")
  {
    return run<v0::Graph, v0::Dijkstras>("dijkstras_v0", graph_json_path, start_vertex_id, goal_vertex_id, iterations);
  }
  else if (version == "v1")
  {
    return run<v1::Graph, v1::Dijkstras>("dijkstras_v1", graph_json_path, start_vertex_id, goal_vertex_id, iterations);
  }
  else if (version == "v2")
  {
    return run<v2::Graph, v2::Dijkstras>("dijkstras_v2", graph_json_path, start_vertex_id, goal_vertex_id, iterations);
  }
  else if (version == "v3")
  {
    return run<v3::Graph, v3::Dijkstras>("dijkstras_v3", graph_json_path, start_vertex_id, goal_vertex_id, iterations);
  }
  else if (version == "v4")
  {
    return run<v4::Graph, v4::Dijkstras>("dijkstras_v4", graph_json_path, start_vertex_id, goal_vertex_id, iterations);
  }
  else
  {
    std::cerr << "Invalid version: " << version << std::endl;
  }

  return retval;
}