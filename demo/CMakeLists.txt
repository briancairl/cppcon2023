add_subdirectory(common)

foreach(VN IN LISTS DEMO_LIST)
  add_subdirectory(${VN})
endforeach()
