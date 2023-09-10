cmake_minimum_required (VERSION 2.8.11)
project (cppcon2023)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -g")

add_subdirectory(core)
add_subdirectory(demo)

add_executable (run_demo run_demo.cpp)
target_link_libraries(run_demo PUBLIC core json v0 v1 vn)