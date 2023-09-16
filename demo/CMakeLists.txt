add_subdirectory(common)

foreach(I RANGE 0 ${DEMO_COUNT})
  add_subdirectory(v${I})
endforeach()
