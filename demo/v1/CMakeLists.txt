add_library(v1 src/graph.cpp src/run.cpp)
target_link_libraries(v1 PUBLIC core json)
target_include_directories(v1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
