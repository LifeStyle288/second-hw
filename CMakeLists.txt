cmake_minimum_required(VERSION 3.2)

project(allocator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(allocator_cli main.cpp)
add_library(factorial factorial.cpp)

target_link_libraries(allocator_cli PRIVATE
    factorial
)
