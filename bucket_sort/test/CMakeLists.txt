cmake_minimum_required(VERSION 3.20)
project(bucket_sort)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

enable_testing()

add_executable(
  bucket_sort_ut
  bucket_sort_ut.cc
)
target_link_libraries(
  bucket_sort_ut
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(bucket_sort_ut)
