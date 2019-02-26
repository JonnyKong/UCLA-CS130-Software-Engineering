include(CMakeParseArguments)

# Find gcovr
find_program(GCOVR_PATH gcovr)

# Defines a target for reporting test code coverage information.
# Builds dependencies, runs tests, and generates a report.
#
# TARGETS should specify all non-test library and executable targets in your project
# TESTS should specify all test executable targets in your project
#
# Run cmake with -DCMAKE_BUILD_TYPE=Coverage and then make coverage to generate
# report
#
# generate_coverage_report(
#     TARGETS [targets...]
#     TESTS [test_targets...]
# )
function(generate_coverage_report)
    # Parse arguments
    set(multiValueArgs TARGETS TESTS)
    cmake_parse_arguments(COVERAGE "" "" "${multiValueArgs}" ${ARGN})

    set(COVERAGE_REPORT_DIR "${CMAKE_BINARY_DIR}/report")
    set(COVERAGE_DATA_STAMP_FILE "${CMAKE_BINARY_DIR}/coverage_data.stamp")
    set(COVERAGE_REPORT_FILE "${COVERAGE_REPORT_DIR}/index.html")

    if(NOT GCOVR_PATH)
        message(FATAL_ERROR "gcovr not found! Aborting...")
    endif()

    string(TOLOWER "${CMAKE_BUILD_TYPE}" LOWER_CMAKE_BUILD_TYPE)
    if("${LOWER_CMAKE_BUILD_TYPE}" STREQUAL "coverage")
        message(STATUS "Coverage will be measured for targets: ${COVERAGE_TARGETS}")

        # Create Coverage build type
        set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG}" PARENT_SCOPE)
        set(CMAKE_C_FLAGS_COVERAGE "${CMAKE_C_FLAGS_DEBUG}" PARENT_SCOPE)
        set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" Coverage PARENT_SCOPE)

        # Add compile/link flags to targets
        foreach(T ${COVERAGE_TARGETS})
            set_property(TARGET ${T} APPEND_STRING PROPERTY COMPILE_FLAGS " -g --coverage")
            set_property(TARGET ${T} APPEND_STRING PROPERTY LINK_FLAGS " -g --coverage")
        endforeach()

        # Tests are not compiled with coverage info, since we do not want them
        # included in the report
        foreach(T ${COVERAGE_TESTS})
            set_property(TARGET ${T} APPEND_STRING PROPERTY LINK_FLAGS " -g --coverage")
        endforeach()

        # Use coverage_data.stamp as a dependency proxy for coverage data files
        add_custom_command(OUTPUT ${COVERAGE_DATA_STAMP_FILE}
            COMMAND find "${CMAKE_BINARY_DIR}" -name "*.gcda" -exec rm {} \;
            COMMAND ${CMAKE_CTEST_COMMAND}
            COMMAND touch "${COVERAGE_DATA_STAMP_FILE}"
            DEPENDS ${COVERAGE_TARGETS} ${COVERAGE_TESTS}
            COMMENT "Running tests to generate coverage data"
            VERBATIM
        )

        # Generage the HTML report, dependent on coverage data
        add_custom_command(OUTPUT ${COVERAGE_REPORT_FILE}
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${COVERAGE_REPORT_DIR}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${COVERAGE_REPORT_DIR}"
            COMMAND "${GCOVR_PATH}" --html --html-details -s
            -r "${CMAKE_SOURCE_DIR}"
            --object-directory "${CMAKE_BINARY_DIR}"
            -o "${COVERAGE_REPORT_FILE}"
            DEPENDS ${COVERAGE_DATA_STAMP_FILE}
            COMMENT "Generating coverage report"
            VERBATIM)
        add_custom_target(coverage DEPENDS ${COVERAGE_REPORT_FILE})

        # Clean up the report directory on make clean
        set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${COVERAGE_REPORT_DIR})
    endif()
endfunction()
