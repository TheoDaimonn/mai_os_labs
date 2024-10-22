cmake_minimum_required(VERSION 3.28)
project(lab2)

set(CMAKE_CXX_STANDARD 17)

# Find and link pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

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

# Main executable
add_executable(lab2 main.cpp src/search_patterns.cpp)
target_include_directories(lab2 PRIVATE include)
target_link_libraries(lab2 PRIVATE Threads::Threads)

# Enable testing and create tests
enable_testing()
add_executable(tests test/tests.cpp src/search_patterns.cpp)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE Threads::Threads GTest::gtest_main)

# GoogleTest integration
include(GoogleTest)
gtest_discover_tests(tests)
