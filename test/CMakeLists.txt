cmake_minimum_required(VERSION 2.8.12)

add_executable(test_all
  test_all.cpp
  test_list_constructors.cpp
  test_list_equal.cpp
  test_list_foreach.cpp
  test_list_map.cpp
  test_list_reverse.cpp
)
target_compile_options(test_all PUBLIC -std=c++11 -Wall -Wextra -Werror -pedantic-errors -O3)
target_include_directories(test_all PUBLIC ../include)
