cmake_minimum_required (VERSION 3.8)

project (v8capi)

set (CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wpedantic -Wconversion")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wpedantic -Wconversion --coverage")

add_definitions (
    -DV8_COMPRESS_POINTERS
    )

set (HEADERS
    include/v8capi.h
    include/v8capi_values.h
    )

set (SOURCES
    src/v8capi.cpp
    src/v8capi_values.cpp
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

find_package (Threads)

target_link_libraries (${CMAKE_PROJECT_NAME}
    v8_monolith
    ${CMAKE_THREAD_LIBS_INIT}
    )

enable_testing ()

configure_file (./tests/data/good_script.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file (./tests/data/infinite_loop.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file (./tests/data/syntax_error.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file (./tests/data/sum.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file (./tests/data/throw.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

macro (register_test test_name)
    set (TEST_NAME test_${test_name})

    add_executable (${TEST_NAME}
        ${HEADERS}
        tests/utils.h tests/utils.cpp
        tests/${TEST_NAME}.cpp
        )

    target_link_libraries (${TEST_NAME}
        ${CMAKE_PROJECT_NAME}
        v8_monolith
        ${CMAKE_THREAD_LIBS_INIT}
        )

    add_test (${TEST_NAME} ${TEST_NAME})
endmacro ()

register_test (common)
register_test (functions)
register_test (multiisolates)
register_test (values)
