#message(STATUS "updateVersion: CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
#message(STATUS "updateVersion: CMAKE_CURRENT_LIST_FILE:  ${CMAKE_CURRENT_LIST_FILE}")
#message(STATUS "updateVersion: CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
#message(STATUS "updateVersion: CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
#message(STATUS "updateVersion: GIT_EXECUTABLE: ${GIT_EXECUTABLE}")

include(inbaDoFindGit)
if (NOT Git_FOUND)
  message(FATAL_ERROR "Cannot update version.hpp without git and Git_FOUND is false")
  return()
endif()


# Grab the version numbers:
include(${CMAKE_CURRENT_SOURCE_DIR}/Version.cmake)
set(INBA_VERSION_VERSION ${INBA_VERSION_MAJOR}.${INBA_VERSION_MINOR}.${INBA_VERSION_PATCHLEVEL})

# Try to build a new version.hpp
configure_file(version.hpp.in include/inba/version.hpp @ONLY)
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/include/inba/version.hpp INBA_OLD_VERSION_HPP)
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/include/inba/version.hpp INBA_NEW_VERSION_HPP)

#message(STATUS "updateVersion: INBA_OLD_VERSION_HPP: ${INBA_OLD_VERSION_HPP}")
#message(STATUS "updateVersion: INBA_NEW_VERSION_HPP: ${INBA_NEW_VERSION_HPP}")

# If the new version.hpp is materially different from the one in the source
# directory, update it, commit, and tag.
if(NOT INBA_NEW_VERSION_HPP STREQUAL INBA_OLD_VERSION_HPP)
  # Check that docs/release_notes.md and Version.cmake are the only changed file:
  execute_process(
    COMMAND ${GIT_EXECUTABLE} -C "${CMAKE_CURRENT_SOURCE_DIR}" status --porcelain -uno
    OUTPUT_VARIABLE INBA_GIT_STATUS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  if ("${INBA_GIT_STATUS}" STREQUAL "")
    # or that there's non change at all because there's no commit yet
  else()
    string(REPLACE "\n" ";"  INBA_GIT_STATUS ${INBA_GIT_STATUS})
    if (NOT "x${INBA_GIT_STATUS}" STREQUAL "x M Version.cmake; M docs/release_notes.md")
      #message(FATAL_ERROR "Cannot update version.hpp: the source directory has a dirty status")
	  message(WARNING "Cannot update version.hpp: the source directory has a dirty status. If you're not testing something consider solving this.")
	  return()
    endif()
  endif()
  
  file(
    COPY ${CMAKE_CURRENT_BINARY_DIR}/include/inba/version.hpp
    DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/inba
  )
  #execute_process(
  #  COMMAND ${GIT_EXECUTABLE} -C "${CMAKE_CURRENT_SOURCE_DIR}" add -u
  #)
  #execute_process(
  #  COMMAND ${GIT_EXECUTABLE} -C "${CMAKE_CURRENT_SOURCE_DIR}" commit -m "${INBA_VERSION_VERSION}"
  #)
  #execute_process(
  #  COMMAND ${GIT_EXECUTABLE} -C "${CMAKE_CURRENT_SOURCE_DIR}" tag -f -a "${INBA_VERSION_VERSION}" -m "${INBA_VERSION_VERSION}"
  #)
  find_program(CONAN_EXECUTABLE NAMES conan conan.exe)
  if (NOT "x${CONAN_EXECUTABLE}" STREQUAL "xCONAN_EXECUTABLE-NOTFOUND")
    message(STATUS "Exporting conanfile for new version")
    #execute_process(
    #  COMMAND ${CONAN_EXECUTABLE} create . inba/${INBA_VERSION_VERSION}@gyzkard/stable
    #  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    #)
  endif()
  message(STATUS "Version updated to ${INBA_VERSION_VERSION}. Don't forget to:")
  message(STATUS "  git push origin <feature-branch>")
  message(STATUS "and (after that is merged to master) then:")
  message(STATUS "  conan remote add inba https://api.bintray.com/conan/inba/inba")
  message(STATUS "  conan create ${CMAKE_CURRENT_SOURCE_DIR} inba/${INBA_VERSION_VERSION}@gyzkard/stable")
  message(STATUS "  conan upload --all inba/${INBA_VERSION_VERSION}@gyzkard/stable -r=inba")
endif()
