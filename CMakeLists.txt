cmake_minimum_required(VERSION 3.8.0)


get_directory_property(is_subproject PARENT_DIRECTORY)
if(NOT is_subproject)
    set(is_standalone YES)
else()
    set(is_standalone NO)
endif()


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Export compilation data-base
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


#set( CMAKE_MESSAGE_LOG_LEVEL "DEBUG" )
#message(DEBUG "main CMakeLists.txt: CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
#message(DEBUG "main CMakeLists.txt: CMAKE_CURRENT_LIST_FILE:  ${CMAKE_CURRENT_LIST_FILE}")
#message(DEBUG "main CMakeLists.txt: CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
#message(DEBUG "main CMakeLists.txt: CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
#message(DEBUG "main CMakeLists.txt: CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
#message(DEBUG "main CMakeLists.txt: PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")


include(updateVersion)


project(inba
  HOMEPAGE_URL "https://github.com/gyzkard/index-base"
  VERSION ${INBA_VERSION_VERSION}
  LANGUAGES CXX)


set(INBA_CPP_STD ${CMAKE_CXX_STANDARD} CACHE STRING "C++ Standard. Supported values are 11, 14, 17, 20, 23")
#message(DEBUG "main CMakeLists.txt: INBA_CPP_STD: ${INBA_CPP_STD}")
#message(DEBUG "main CMakeLists.txt: CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
#message(DEBUG "main CMakeLists.txt: CMAKE_CXX_STANDARD_REQUIRED: ${CMAKE_CXX_STANDARD_REQUIRED}")


include(CMakeDependentOption)
include(GNUInstallDirs)
include(CTest)

cmake_dependent_option(INBA_BUILD_DOCS "Build inba Docs" ON
  "CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR" OFF)
cmake_dependent_option(INBA_BUILD_TESTS "Build inba Tests" ON
  "BUILD_TESTING;CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR" OFF)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
# target_compile_features(inba INTERFACE cxx_std_20)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets)

install(EXPORT ${PROJECT_NAME}-targets 
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}"
  NAMESPACE ${PROJECT_NAME}::)
install(
  FILES
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME})
install(DIRECTORY include/ DESTINATION include
    FILES_MATCHING PATTERN *.hpp)


if(INBA_BUILD_DOCS)
  include(doFindDoxygen)
  add_subdirectory(docs)
endif()

if (INBA_BUILD_TESTS)
  add_subdirectory(tests)
endif()
