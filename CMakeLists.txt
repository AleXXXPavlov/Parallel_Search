cmake_minimum_required(VERSION 3.20)
project(Parallel_Search)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(Parallel_Search main.cpp Arguments.cpp Arguments.h SearchEngine.cpp SearchEngine.h FindPattern.cpp FindPattern.h)
