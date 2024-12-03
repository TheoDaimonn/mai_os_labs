cmake_minimum_required(VERSION 3.28)
project(Lab4)

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

# Указание директорий с заголовочными файлами
include_directories(include)

# Добавление библиотек
add_library(gcf SHARED src/gcf_euclid.cpp src/gcf_naive.cpp)
add_library(square SHARED src/square_rectangle.cpp src/square_triangle.cpp)

# Добавление исполняемых файлов
add_executable(main1 main1.cpp)
target_link_libraries(main1 gcf square)

add_executable(main2 main2.cpp)
target_link_libraries(main2 dl)

# Настройка Google Test
enable_testing()
add_executable(tests tests/test.cpp)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE GTest::gtest_main gcf square dl)

include(GoogleTest)
gtest_discover_tests(tests)
