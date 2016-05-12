cmake_minimum_required(VERSION 2.6)

include_directories(. ../include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror -pedantic-errors -O0 -g")

add_executable(test_all
  test_all.cpp

  List/test_constructors.cpp
  List/test_equal.cpp
  List/test_head.cpp
  List/test_tail.cpp
  List/test_uncons.cpp
  List/test_last.cpp
  List/test_init.cpp
  List/test_foreach.cpp
  List/test_map.cpp
  List/test_filter.cpp
  List/test_filter_not.cpp
  List/test_reverse.cpp
  List/test_take.cpp
  List/test_take_right.cpp
  List/test_take_while.cpp
  List/test_drop.cpp
  List/test_drop_right.cpp
  List/test_drop_while.cpp
  List/test_slice.cpp
  List/test_flat_map.cpp
  List/test_flatten.cpp
  List/test_exists.cpp
  List/test_forall.cpp
  List/test_contains.cpp
  List/test_count.cpp
  List/test_prepend.cpp
  List/test_concat.cpp
  List/test_updated.cpp
  List/test_fold.cpp
  List/test_fold_left.cpp
  List/test_fold_right.cpp
  List/test_sum.cpp
  List/test_product.cpp
  List/test_sorted.cpp
  List/test_cref.cpp
  List/test_zip.cpp
  List/test_scan.cpp
  List/test_scan_left.cpp
  List/test_scan_right.cpp
  List/test_reduce.cpp
  List/test_reduce_left.cpp
  List/test_reduce_right.cpp
  List/test_begin_end.cpp

  Option/test_constructors.cpp
  Option/test_foreach.cpp
  Option/test_map.cpp
  Option/test_unowned_constructors.cpp
  Option/test_unowned_foreach.cpp
  Option/test_unowned_map.cpp

  lazy/test_lazy_val.cpp
)
