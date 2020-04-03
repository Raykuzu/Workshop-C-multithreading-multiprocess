cmake_minimum_required(VERSION 3.5)

project(c-multithreading)

set(CMAKE_C_FLAGS "-W -Wall -Wextra -std=c11")

add_executable(${PROJECT_NAME}-base base.c)
target_link_libraries(${PROJECT_NAME}-base pthread)

add_executable(${PROJECT_NAME}-attribute attribute.c)
target_link_libraries(${PROJECT_NAME}-attribute pthread)

add_executable(${PROJECT_NAME}-detach detach.c)
target_link_libraries(${PROJECT_NAME}-detach pthread)

add_executable(${PROJECT_NAME}-quit quit.c)
target_link_libraries(${PROJECT_NAME}-quit pthread)

add_executable(${PROJECT_NAME}-mutex mutex.c)
target_link_libraries(${PROJECT_NAME}-mutex pthread)

add_executable(${PROJECT_NAME}-deadlock deadlock.c)
target_link_libraries(${PROJECT_NAME}-deadlock pthread)