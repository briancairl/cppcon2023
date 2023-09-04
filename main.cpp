// C++ Standard Library
#include <sstream>
#include <fstream>

// Unix
#include <sched.h>

// PicoJSON
#include <picojson/picojson.h>

// CPPCon
#include "dijkstras.h"

using namespace cppcon;

vertex_id_t to_vertex_id(const char* const vertex_id_str)
{
  vertex_id_t vertex_id;
  std::stringstream ss;
  ss << vertex_id_str;
  ss >> vertex_id;
  return vertex_id;
}

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

int main(int argc, char** argv)
{
  if (argc < 5)
  {
    return 1;
  }

  cpu_set_t  mask;
  CPU_ZERO(&mask);
  CPU_SET(0, &mask);
  if (const int result = sched_setaffinity(0, sizeof(mask), &mask); result != 0)
  {
    return 1;
  }


  const auto graph = Graph::read(argv[1]);

  Dijkstras opt{graph};

  const auto start_vertex_id = to_vertex_id(argv[2]);

  const auto goal_vertex_id = to_vertex_id(argv[3]);

  if (opt.search(graph, start_vertex_id, goal_vertex_id))
  {
    dump_path(argv[4], opt.get_path(goal_vertex_id));
    return 0;
  }
  else
  {
    return 1;
  }
}