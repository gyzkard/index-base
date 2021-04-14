# see https://foonathan.net/2016/07/cmake-dependency-handling/

set(gympl_relative_path "external/gy-mpl")
set(gympl_main_header "gympl.hpp")
set(gympl_main_header_expected_path "${CMAKE_CURRENT_SOURCE_DIR}/${gympl_relative_path}/include/gympl")

find_path(INBA_GYMPL_INCLUDE_DIR NAMES "${gympl_main_header}" PATHS "${gympl_main_header_expected_path}")
#message("doFindGympl: gympl_relative_path: ${gympl_relative_path}")
#message("doFindGympl: gympl_main_header: ${gympl_main_header}")
#message("doFindGympl: gympl_main_header_expected_path: ${gympl_main_header_expected_path}")
#message("doFindGympl: INBA_GYMPL_INCLUDE_DIR: ${INBA_GYMPL_INCLUDE_DIR}")
#message("doFindGympl: CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

if((NOT INBA_GYMPL_INCLUDE_DIR) OR (NOT EXISTS ${INBA_GYMPL_INCLUDE_DIR}))
    # we couldn't find the header files for GYMPL or they don't exist
    message("Unable to find gympl while looking for ${gympl_main_header} in ${gympl_main_header_expected_path}. Trying to update the submodule.")

    # we have a submodule setup for gympl, assume it is under external/gy-mpl
    # now we need to clone this submodule
    execute_process(COMMAND git submodule update --init -- ${gympl_relative_path} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} RESULT_VARIABLE git_sm_upd_init_gympl_result)
	message("doFindGympl: execute_process git update sm gympl ret: ${git_sm_upd_init_gympl_result}")

	#todo:c/p
	find_path(INBA_GYMPL_INCLUDE_DIR NAMES "${gympl_main_header}" PATHS "${gympl_main_header_expected_path}")
else()
endif()

if((NOT INBA_GYMPL_INCLUDE_DIR) OR (NOT EXISTS ${INBA_GYMPL_INCLUDE_DIR}))
    message("Looks like something went wrong while trying to update the gympl submodule.")
else()
	add_subdirectory("${gympl_relative_path}" EXCLUDE_FROM_ALL)
	#add_subdirectory("${gympl_relative_path}")
	#add_subdirectory(external/gy-mpl "${CMAKE_CURRENT_BINARY_DIR}/gympl" EXCLUDE_FROM_ALL)
	#set_target_properties("gympl-test" PROPERTIES EXCLUDE_FROM_ALL 1)
	
	unset(GYMPL_CPP_STD CACHE)
	set(GYMPL_CPP_STD "${INBA_CPP_STD}")
endif()
