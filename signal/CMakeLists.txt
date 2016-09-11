cmake_minimum_required(VERSION 2.6)

set (TEST_SIGNAL_LIB "test")
set (SIGNAL_LIB "er_signal")

project(${TEST_SIGNAL_LIB})

set(LIB_INSTALL_PATH /usr/lib/)
set(HEADERS_INSTALL_PATH /usr/include)

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
endif(CrossC)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2 -g")

set(HEADERS_SIGNAL er_signal.h)

set(SOURCES_TEST test.c)
set(SOURCES_SIGNAL_LIB er_signal.c ../general/gpio.c)

add_executable(${TEST_SIGNAL_LIB} ${SOURCES_TEST})
target_link_libraries(${TEST_SIGNAL_LIB} ${SIGNAL_LIB})

add_library (${SIGNAL_LIB} STATIC ${SOURCES_SIGNAL_LIB})
target_link_libraries(${SIGNAL_LIB} pthread)


install(TARGETS ${SIGNAL_LIB} DESTINATION ${LIB_INSTALL_PATH})
install(FILES ${HEADERS_SIGNAL} DESTINATION ${HEADERS_INSTALL_PATH})