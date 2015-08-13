cmake_minimum_required(VERSION 2.6)

set (PROJECT "test")

project(${PROJECT})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(HEADERS wheels.h)
set(SOURCES test.c wheels.c)

add_executable(${PROJECT} ${HEADERS} ${SOURCES})