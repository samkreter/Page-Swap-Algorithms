cmake_minimum_required (VERSION 2.8)
project (page_swap)

set(CMAKE_CXX_FLAGS "--std=c++11 -g -Werror")

find_library(blockstore_lib block_store)

add_executable(runner test/runner.cpp)
target_link_libraries(runner blockstore_lib)

set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Werror -O3")
add_executable(page_generator src/page_generator.cpp)
