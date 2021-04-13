file(GLOB test-sources CONFIGURE_DEPENDS *.cpp)

add_custom_target(inba-test)

enable_testing()

foreach (source IN LISTS test-sources)
  get_filename_component(test "${source}" NAME_WE)

  add_executable(inba-test-${test} ${source} ${ARGN})
  add_test(inba-test-${test} inba-test-${test})

  set_property(TARGET inba-test-${test} PROPERTY CXX_STANDARD ${INBA_CPP_STD})
  target_compile_features(inba-test-${test} INTERFACE cxx_std_11)
  target_link_libraries(inba-test-${test} PRIVATE inba)
  add_dependencies(inba-test inba-test-${test})
endforeach()
