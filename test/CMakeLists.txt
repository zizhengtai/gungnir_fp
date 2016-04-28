cmake_minimum_required(VERSION 2.6)

include_directories(../include)

add_definitions("-std=c++11 -Wall -Wextra -Werror -pedantic-errors -O3")
add_executable(test_all
  test_all.cpp
  test_list_constructors.cpp
)
