cmake_minimum_required (VERSION 3.20.0)
project (cppcon2023)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -g -DNDEBUG -D_DEBUG")

set(DEMO_LIST "v3")

# AUTO-GENERATE DEMOS TO INCLUDE
file(WRITE auto_generated_includes.h "// --- AUTO-GENERATED ---\n")
foreach(VN IN LISTS DEMO_LIST)
  file(APPEND auto_generated_includes.h
       "#include \"cppcon/demo/${VN}/run.h\"\n"
  )
endforeach()
file(APPEND auto_generated_includes.h "// --- AUTO-GENERATED ---\n")


# AUTO-GENERATE COMMANDS TO RUN
file(WRITE auto_generated_commands.h "// --- AUTO-GENERATED ---\n#include <filesystem>\n#define RUN_ALL_DEMOS(in, out, settings) \\\n")
foreach(VN IN LISTS DEMO_LIST)
  file(APPEND auto_generated_commands.h
       "::cppcon::demo::${VN}::run(in, ::std::filesystem::path{out}.replace_extension(\".${VN}.json\"), settings);\\\n"
  )
endforeach()
file(APPEND auto_generated_commands.h "// --- AUTO-GENERATED ---\n")

add_subdirectory(core)
add_subdirectory(demo)

add_executable (run_demo run_demo.cpp)
target_include_directories(run_demo PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(run_demo PUBLIC core json ${DEMO_LIST})