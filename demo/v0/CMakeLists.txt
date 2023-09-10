add_library(v0 src/graph.cpp src/run.cpp)
target_link_libraries(v0 PUBLIC core json)
target_include_directories(v0 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
