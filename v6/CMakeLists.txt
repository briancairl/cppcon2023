add_library (dijkstras_v6 src/dijkstras.cpp)
target_include_directories (dijkstras_v6 PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/common
)

add_library (dijkstras_v6_json src/dijkstras_json.cpp)
target_include_directories (dijkstras_v6_json PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/thirdparty
  ${PROJECT_SOURCE_DIR}/common
)
