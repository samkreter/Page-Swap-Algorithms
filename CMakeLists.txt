cmake_minimum_required (VERSION 2.8)
project (page_swap)

set(CMAKE_CXX_FLAGS "--std=c++0x -g -Werror")

find_library(blockstore_lib block_store)

add_library(swap_algorithms src/page_swap.cpp src/LRU.cpp src/ALRU.cpp)
target_link_libraries(swap_algorithms ${blockstore_lib} )

add_executable(runner test/runner.cpp)
target_link_libraries(runner ${blockstore_lib} swap_algorithms)

set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Werror -O3")
add_executable(page_generator src/page_generator.cpp)
