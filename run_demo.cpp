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
  if (argc != 4)
  {
    std::cerr << argv[0] << " <graph_json> <output_json> <percentage or problems>" << std::endl;
    return 1;
  }

  const float percentage_of_problems = to<float>(argv[3]) / 100.0;

  demo::v0::run(argv[1], std::filesystem::path{argv[2]}.replace_extension(".v0.json"), percentage_of_problems);
  demo::v1::run(argv[1], std::filesystem::path{argv[2]}.replace_extension(".v1.json"), percentage_of_problems);
  demo::v2::run(argv[1], std::filesystem::path{argv[2]}.replace_extension(".v2.json"), percentage_of_problems);
  demo::vn::run(argv[1], std::filesystem::path{argv[2]}.replace_extension(".vn.json"), percentage_of_problems);

  return 0;
}