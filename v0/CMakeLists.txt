add_library (dijkstras_v0 src/dijkstras.cpp)
target_include_directories (dijkstras_v0 PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/common
)

add_library (dijkstras_v0_json src/dijkstras_json.cpp)
target_include_directories (dijkstras_v0_json PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/thirdparty
  ${PROJECT_SOURCE_DIR}/common
)
