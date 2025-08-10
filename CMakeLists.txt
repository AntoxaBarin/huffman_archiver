cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_COMPILER "/usr/bin/g++-13")

project(
  ModernCMakeExample
  VERSION 0.1
  LANGUAGES CXX)


include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

set(SOURCE 
    src/main.cpp
    src/huffman_tree.cpp
    src/encoding.cpp
)

set(TEST_SOURCE 
    test/test.cpp
    test/doctest.h
    src/huffman_tree.cpp
    src/encoding.cpp
)

add_compile_options(-O2 -Wall -Werror -Wextra  -std=c++17)

add_executable(huffman_archiver ${SOURCE})
add_executable(huffman_test ${TEST_SOURCE})
