get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_library(${TARGET} src/graph.cpp src/run.cpp)
target_link_libraries(${TARGET} PUBLIC core json)
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
