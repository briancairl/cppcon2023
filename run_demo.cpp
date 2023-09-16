#ifdef __linux__    // Linux only
#include <sched.h>  // sched_setaffinity
#endif

// C++ Standard Library
#include <iostream>
#include <sstream>

// CPPCon
#include "auto_generated_includes.h"
#include "auto_generated_commands.h"

using namespace cppcon;

void cpu_pin(int cpu)
{
  #ifdef __linux__
    cpu_set_t mask;
    int status;

    CPU_ZERO(&mask);
    CPU_SET(cpu, &mask);
    status = sched_setaffinity(0, sizeof(mask), &mask);
    if (status != 0)
    {
      perror("sched_setaffinity");
    }
  #endif
}

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
    std::cerr << argv[0] << " <graph_json> <output_json> [<percentage or problems>] [<shuffle_seed>]" << std::endl;
    return 1;
  }

  cpu_pin(1);

  const demo::Settings settings{
    .percentage_of_problems = (argc > 3) ? (to<float>(argv[3]) / 100.f) : 0.1f,
    .shuffle_seed = (argc > 4) ? to<std::size_t>(argv[4]) : 0
  };

  RUN_ALL_DEMOS(argv[1], argv[2], settings);

  return 0;
}