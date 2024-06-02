project(Redis-Analog)

add_library(server STATIC server.cpp)
add_library(storage STATIC storage.cpp)
add_library(logger STATIC logger.cpp)

add_executable(Redis-Analog main.cpp)

set_target_properties(Redis-Analog PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")

target_link_libraries(Redis-Analog server storage logger)