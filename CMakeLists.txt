cmake_minimum_required (VERSION 2.8.11)
project (cppcon2023)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -g")

add_subdirectory(v0)
add_subdirectory(v1)
add_subdirectory(v2)
add_subdirectory(v3)
add_subdirectory(v4)
add_subdirectory(v5)

add_executable (main main.cpp)
target_link_libraries(main PUBLIC
  dijkstras_v0 dijkstras_v0_json
  dijkstras_v1 dijkstras_v1_json
  dijkstras_v2 dijkstras_v2_json
  dijkstras_v3 dijkstras_v3_json
  dijkstras_v4 dijkstras_v4_json
  dijkstras_v5 dijkstras_v5_json
)