cmake_minimum_required (VERSION 3.8)

project (v8capi)

set (V8_LIBRARY thirdparty/v8/libv8_monolith.a)

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
    thirdparty/v8/include
    )

link_directories (
    thirdparty/v8
    )

add_library (${CMAKE_PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
    )

if (${V8CAPI_BUILD_V8})
    add_custom_target (build_v8
        COMMAND thirdparty/build_v8.sh
        BYPRODUCTS thirdparty/v8/libv8_monolith.a
        COMMENT "Building V8"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        VERBATIM
        )

    add_dependencies (${CMAKE_PROJECT_NAME} build_v8)
endif ()

find_package (Threads)

target_link_libraries (${CMAKE_PROJECT_NAME}
    v8_monolith
    ${CMAKE_THREAD_LIBS_INIT}
    )

add_subdirectory (thirdparty/googletest/googletest)

include_directories (thirdparty/googletest/googletest/include)

enable_testing ()

configure_file (./tests/data/good_script.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file (./tests/data/infinite_loop.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file (./tests/data/primitives.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file (./tests/data/syntax_error.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file (./tests/data/sum.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file (./tests/data/throw.js ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

set (TEST_NAME test_${CMAKE_PROJECT_NAME})

add_executable (${TEST_NAME}
    ${HEADERS}

    tests/isolate_fixture.h tests/isolate_fixture.cpp
    tests/utils.h tests/utils.cpp

    tests/main.cpp

    tests/test_conversions.cpp
    tests/test_common.cpp
    tests/test_functions.cpp
    tests/test_multiisolates.cpp
    tests/test_values.cpp
    )

target_link_libraries (${TEST_NAME}
    ${CMAKE_PROJECT_NAME}
    v8_monolith
    ${CMAKE_THREAD_LIBS_INIT}
    gtest
    )

add_test (${TEST_NAME} ${TEST_NAME})
