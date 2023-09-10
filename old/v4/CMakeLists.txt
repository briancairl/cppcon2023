add_library (dijkstras_v4 src/dijkstras.cpp)
target_include_directories (dijkstras_v4 PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/common
)

add_library (dijkstras_v4_json src/dijkstras_json.cpp)
target_include_directories (dijkstras_v4_json PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/thirdparty
  ${PROJECT_SOURCE_DIR}/common
)
