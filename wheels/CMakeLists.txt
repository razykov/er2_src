cmake_minimum_required(VERSION 2.6)

set (PROJECT "test")
set (PROJECT_LIB "wheels")

project(${PROJECT})

option(CrossC "CrossC")

if(CrossC)
message("Cross compiling")

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER   "/opt/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-gcc")
set(CMAKE_CXX_COMPILER "/opt/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-g++")

set(CMAKE_FIND_ROOT_PATH  "/opt/arm-bcm2708-linux-gnueabi")
include_directories ("/opt/arm-bcm2708-linux-gnueabi/wiringPi/wiringPi")
link_directories("/opt/arm-bcm2708-linux-gnueabi")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif(__CROSSC__)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")

set(HEADERS wheels.h er_errors.h)
set(SOURCES test.c)
set(SOURCES_LIB wheels.c )

add_library (${PROJECT_LIB} SHARED ${HEADERS} ${SOURCES_LIB})
target_link_libraries(${PROJECT_LIB} wiringPi)
target_link_libraries(${PROJECT_LIB} pthread)

add_executable(${PROJECT} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT} ${PROJECT_LIB} )