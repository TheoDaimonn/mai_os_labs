cmake_minimum_required(VERSION 3.28)
project(InterProcessCommunication)

set(CMAKE_CXX_STANDARD 17)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
  TLS_VERIFY FALSE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Main parent process executable
add_executable(parent main.cpp src/parent.cpp src/utils.cpp)
target_include_directories(parent PRIVATE include)

# Child1 and Child2 executables
add_executable(child1 src/child1.cpp)
target_include_directories(child1 PRIVATE include)

add_executable(child2 src/child2.cpp)
target_include_directories(child2 PRIVATE include)

# Google Test setup for testing
enable_testing()
add_executable(tests test/test.cpp src/parent.cpp src/utils.cpp)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
