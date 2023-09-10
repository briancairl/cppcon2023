add_library(v2 src/graph.cpp src/run.cpp)
target_link_libraries(v2 PUBLIC core json)
target_include_directories(v2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
