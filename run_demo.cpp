// C++ Standard Library
#include <filesystem>
#include <chrono>
#include <iostream>
#include <sstream>

// CPPCon
#include "cppcon/demo/v0/run.h"
#include "cppcon/demo/v1/run.h"
#include "cppcon/demo/v2/run.h"
#include "cppcon/demo/vn/run.h"

using namespace cppcon;

template<typename T>
T to(std::string_view str)
{
  T value;
  std::stringstream ss;
  ss << str;
  ss >> value;
  return value;
}


int main(int argc, char** argv)
{
  if (argc < 3)
  {
    std::cerr << argv[0] << " <graph_json> <output_json> [<percentage or problems>] [<shuffles>]" << std::endl;
    return 1;
  }


  const demo::Settings settings{
    .percentage_of_problems = (argc > 3) ? (to<float>(argv[3]) / 100.f) : 0.1f,
    .shuffles = (argc > 4) ? to<int>(argv[4]) : 1
  };

  demo::v0::run(argv[1], std::filesystem::path{argv[2]}.replace_extension(".v0.json"), settings);
  demo::v1::run(argv[1], std::filesystem::path{argv[2]}.replace_extension(".v1.json"), settings);
  demo::v2::run(argv[1], std::filesystem::path{argv[2]}.replace_extension(".v2.json"), settings);
  demo::vn::run(argv[1], std::filesystem::path{argv[2]}.replace_extension(".vn.json"), settings);

  return 0;
}