cmake_minimum_required(VERSION 3.5)

project(c-multiprocess)

set(CMAKE_C_FLAGS "-W -Wall -Wextra -std=c11")

add_executable(${PROJECT_NAME}-create create.c)

add_executable(${PROJECT_NAME}-simple_stop simple_stop.c)

add_executable(${PROJECT_NAME}-cascade_stop cascade_stop.c)

add_executable(${PROJECT_NAME}-named_pipe named_pipe.c)

add_executable(${PROJECT_NAME}-posix_shared_memory posix_shared_memory.c)
target_link_libraries(${PROJECT_NAME}-posix_shared_memory rt)