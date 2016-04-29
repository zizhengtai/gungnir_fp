cmake_minimum_required(VERSION 2.6)

include_directories(../include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror -pedantic-errors -O0 -g")
add_executable(test_all
  test_all.cpp
  test_list_constructors.cpp
  test_list_equal.cpp
  test_list_foreach.cpp
  test_list_map.cpp
  test_list_reverse.cpp
)
