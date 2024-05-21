project(Redis-Analog)

include_directories("sources/")

add_library(server STATIC sources/server.cpp)
add_library(storage STATIC sources/storage.cpp)
add_library(logger STATIC sources/logger.cpp)

add_executable(Redis-Analog sources/main.cpp)

target_link_libraries(Redis-Analog server storage logger)