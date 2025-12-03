cmake_minimum_required(VERSION 3.14)
project(MyCppProject LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------
# Fetch GoogleTest using FetchContent (no redundant add_subdirectory)
# --------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)

# This downloads and makes the targets (GTest::gtest, GTest::gtest_main, ...)
# available for linking.
FetchContent_MakeAvailable(googletest)

# --------------------------------------------------------------------
# Project library + executable separation
# - Build a library for the core code so tests can link to it without
#   pulling in the application's main().
# --------------------------------------------------------------------

# Core library sources (implementation files only)
add_library(MyCppProjectLib
)

target_include_directories(MyCppProjectLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)


# --------------------------------------------------------------------
# Testing - automatic discovery of tests under tests/*.cpp
# - Only creates the test executable if test sources exist.
# --------------------------------------------------------------------
enable_testing()

# Find test sources (zero or more)
file(GLOB_RECURSE TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "tests/*.cpp" "tests/*.cc" "tests/*.cxx")

if(TEST_SOURCES)
    add_executable(MyCppProject_tests ${TEST_SOURCES})

    target_include_directories(MyCppProject_tests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    # Link tests against GoogleTest and the project library
    target_link_libraries(MyCppProject_tests
        PRIVATE
        GTest::gtest_main
        MyCppProjectLib
    )

    # Use CMake's GoogleTest integration to register tests
    include(GoogleTest)
    gtest_discover_tests(MyCppProject_tests)
else()
    message(STATUS "No test sources found in 'tests/'. To enable tests, add test .cpp files under tests/")
endif()

# --------------------------------------------------------------------
# Useful options / notes
# --------------------------------------------------------------------
# - If you prefer a shared library instead of static, change add_library to SHARED.
# - Keep headers in src/ and include them via target_include_directories (done above).
# - To add tests, create files like tests/test_emitter.cpp and they will be
#   automatically picked up on next CMake configure.
