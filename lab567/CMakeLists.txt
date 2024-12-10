cmake_minimum_required(VERSION 3.10)
project(Lab567)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find ZeroMQ library and include directories
find_path(ZMQ_INCLUDE_DIR zmq.hpp
    HINTS /usr/include /usr/local/include /opt/local/include)

find_library(ZMQ_LIBRARY zmq
    HINTS /usr/lib /usr/local/lib /opt/local/lib)

if(ZMQ_INCLUDE_DIR AND ZMQ_LIBRARY)
    set(ZMQ_FOUND TRUE)
    set(ZMQ_INCLUDE_DIRS ${ZMQ_INCLUDE_DIR})
    set(ZMQ_LIBRARIES ${ZMQ_LIBRARY})
else()
    set(ZMQ_FOUND FALSE)
endif()

if(ZMQ_FOUND)
    message(STATUS "Found ZeroMQ: includes at ${ZMQ_INCLUDE_DIRS}, library at ${ZMQ_LIBRARIES}")
else()
    message(FATAL_ERROR "ZeroMQ not found")
endif()

include_directories(${ZMQ_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Define source files
set(SERVER_SOURCES
    ${PROJECT_SOURCE_DIR}/src/server.cpp
    ${PROJECT_SOURCE_DIR}/src/node_routine.cpp
    ${PROJECT_SOURCE_DIR}/src/socket_routine.cpp
)

set(CLIENT_SOURCES
    ${PROJECT_SOURCE_DIR}/src/client.cpp
    ${PROJECT_SOURCE_DIR}/src/node_routine.cpp
    ${PROJECT_SOURCE_DIR}/src/socket_routine.cpp
)

set(TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/test/tests.cpp
    ${PROJECT_SOURCE_DIR}/src/node_routine.cpp
    ${PROJECT_SOURCE_DIR}/src/socket_routine.cpp
)

# Add executables and link libraries
add_executable(server ${SERVER_SOURCES})
target_link_libraries(server ${ZMQ_LIBRARIES})

add_executable(client ${CLIENT_SOURCES})
target_link_libraries(client ${ZMQ_LIBRARIES})

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests gtest gtest_main pthread ${ZMQ_LIBRARIES})

# Google Test configuration
enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# Add tests
add_test(NAME Lab567Tests COMMAND tests)