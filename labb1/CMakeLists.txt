cmake_minimum_required(VERSION 3.28)
project(lab1)

set(CMAKE_CXX_STANDARD 17)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
  TLS_VERIFY false
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(lab1 main.cpp src/parent.cpp src/utils.cpp)
target_include_directories(lab1 PRIVATE include)

add_executable(child1 src/child1.cpp)
target_include_directories(child1 PRIVATE include)

add_executable(child2 src/child2.cpp)
target_include_directories(child2 PRIVATE include)

enable_testing()
add_executable(tests test/test.cpp src/parent.cpp include/parent.hpp src/utils.cpp include/utils.hpp)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
