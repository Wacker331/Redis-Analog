cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
project(Redis-Analog)

enable_testing()

add_subdirectory(sources)
add_subdirectory(tests)
