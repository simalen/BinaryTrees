cmake_minimum_required(VERSION 3.20)
project(TestingC2 C)

set(CMAKE_C_STANDARD 99)

add_executable(TestingC2 BSTree.c BSTree.h test_BSTree.c)
