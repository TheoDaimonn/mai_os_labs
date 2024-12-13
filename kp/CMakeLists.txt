cmake_minimum_required(VERSION 3.10)
project(kp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

include_directories(${ZMQ_INCLUDE_DIRS} include)

add_executable(server src/server.cpp src/game.cpp src/zmq_utils.cpp)
target_link_libraries(server ${ZMQ_LIBRARIES})

add_executable(client src/client.cpp src/zmq_utils.cpp)
target_link_libraries(client ${ZMQ_LIBRARIES})

enable_testing()
add_executable(tests tests/tests.cpp src/game.cpp)
target_link_libraries(tests gtest gtest_main pthread ${ZMQ_LIBRARIES})

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

add_test(NAME kpTests COMMAND tests)
