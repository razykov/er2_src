cmake_minimum_required(VERSION 2.6)

set (PROJECT "test")
set (WHEELS_LIB "wheels")
set (MCONTROL_LIB "mcontrol")

project(${PROJECT})

set(INSTALL_PATH /usr/bin/)
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

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -O2")

set(HEADERS wheels.h er_errors.h)
set(HEADERS_MCONTROL machine_control.h)
set(HEADERS_INSTALL wheels.h machine_control.h)

set(SOURCES test.c ../general/gpio.c)
set(SOURCES_WHEELS_LIB wheels.c ../general/gpio.c)
set(SOURCES_MCONTROL_LIB machine_control.c)

add_library (${WHEELS_LIB} SHARED ${HEADERS} ${SOURCES_WHEELS_LIB})
target_link_libraries(${WHEELS_LIB} wiringPi)
target_link_libraries(${WHEELS_LIB} pthread)

add_library (${MCONTROL_LIB} SHARED ${HEADERS} ${HEADERS_MCONTROL} ${SOURCES_MCONTROL_LIB})
target_link_libraries(${MCONTROL_LIB} ${WHEELS_LIB})

add_executable (${PROJECT} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT} ${WHEELS_LIB} ${MCONTROL_LIB})


install(TARGETS ${WHEELS_LIB} DESTINATION ${LIB_INSTALL_PATH})
install(TARGETS ${MCONTROL_LIB} DESTINATION ${LIB_INSTALL_PATH})
install(FILES ${HEADERS_INSTALL} DESTINATION ${HEADERS_INSTALL_PATH})