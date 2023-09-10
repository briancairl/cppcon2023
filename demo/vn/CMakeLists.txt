add_library(vn src/graph.cpp src/run.cpp)
target_link_libraries(vn PUBLIC core json)
target_include_directories(vn PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
