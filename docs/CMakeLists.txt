#=============================================================================
# Setup the documentation
#=============================================================================
if (NOT DOXYGEN_FOUND)
    message(STATUS
    "Doxygen not found; the 'doc' and 'gh-pages.{clean,copy,update}' targets "
    "will be unavailable.")
    return()
endif()

set(CMAKE_FOLDER "docs")

configure_file(Doxyfile.in Doxyfile @ONLY)
#configure_file(preprocess.sh.in preprocess.sh @ONLY)
add_custom_target(inba.doc.check
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
    COMMENT "Running Doxygen to validate the documentation"
    VERBATIM
)
set_target_properties(inba.doc.check
    PROPERTIES FOLDER ${CMAKE_FOLDER}
)

add_custom_target(inba-doc
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
)
set_target_properties(inba-doc
    PROPERTIES FOLDER ${CMAKE_FOLDER}
)

if (NOT GIT_FOUND)
    message(STATUS
    "Git was not found; the 'gh-pages.{clean,copy,update}' targets "
    "will be unavailable.")
    return()
endif()

add_custom_target(inba-gh-pages.clean
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/clean-gh-pages.cmake
    COMMAND ${CMAKE_COMMAND} -E remove_directory search
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gh-pages
    COMMENT "Cleaning up docs/gh-pages"
    VERBATIM
)
set_target_properties(inba-gh-pages.clean
    PROPERTIES FOLDER ${CMAKE_FOLDER}
)

add_custom_target(inba-gh-pages.copy
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/html ${CMAKE_CURRENT_LIST_DIR}/gh-pages
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gh-pages
    COMMENT "Copying the documentation from ${CMAKE_CURRENT_BINARY_DIR}/html to docs/gh-pages"
    DEPENDS inba-doc inba-gh-pages.clean
    VERBATIM
)
set_target_properties(inba-gh-pages.copy
    PROPERTIES FOLDER ${CMAKE_FOLDER}
)

execute_process(
    COMMAND ${GIT_EXECUTABLE} -C ${CMAKE_SOURCE_DIR} rev-parse --short HEAD
    OUTPUT_VARIABLE INBA_GIT_SHORT_SHA
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

#message(DEBUG "docs CMakeLists.txt: INBA_GIT_SHORT_SHA: ${INBA_GIT_SHORT_SHA}")
add_custom_target(inba-gh-pages.update
    COMMAND ${GIT_EXECUTABLE} add --all .
    COMMAND ${GIT_EXECUTABLE} commit -m "Update to ${INBA_GIT_SHORT_SHA}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gh-pages
    COMMENT "Updating the gh-pages branch with freshly built documentation"
    DEPENDS inba-gh-pages.copy
    VERBATIM
)
set_target_properties(inba-gh-pages.update
    PROPERTIES FOLDER ${CMAKE_FOLDER}
)
