add_library(json src/json.cpp src/run.cpp)
target_link_libraries(json PUBLIC core)
target_include_directories(json
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/thirdparty
)

