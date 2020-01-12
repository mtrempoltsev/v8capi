cmake_minimum_required (VERSION 3.8)

project (v8capi)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

add_definitions (
    -DV8_COMPRESS_POINTERS
    )

set (HEADERS
    v8capi.h
    )

set (SOURCES
    v8capi.cpp
    )

include_directories (
    ${V8CAPI_LIB_ROOT}/v8/include
    )

link_directories (
    ${V8CAPI_LIB_ROOT}/v8/linux.x64
    )

add_library (${CMAKE_PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
    )

target_link_libraries (${CMAKE_PROJECT_NAME}
    v8_monolith
    pthread
    )

set (TEST_SOURCES
    tests/test_simple.cpp
    )

set (TEST_NAME test_${CMAKE_PROJECT_NAME})

configure_file (./tests/data/good_script.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file (./tests/data/infinite_loop.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file (./tests/data/syntax_error.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file (./tests/data/throw.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable (${TEST_NAME}
    ${HEADERS}
    ${TEST_SOURCES}
    )

target_link_libraries (${TEST_NAME}
    ${CMAKE_PROJECT_NAME}
    v8_monolith
    pthread
    )

enable_testing ()

add_test (tests ${TEST_NAME})
