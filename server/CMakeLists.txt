cmake_minimum_required(VERSION 2.6)

set (PROJECT "er_server")

project(${PROJECT})

option(CrossC "CrossC")

set(INSTALL_PATH /usr/bin/)
set(CONTENT_INSTALL_PATH /var/www/er_server/)

set(CONTENT content/)
set(STREAMER er_stream)
set(GETDBM get_dbm)
set(SAY say)

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

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")

#set(HEADERS server.h)

set(SOURCES er_server.c)

add_executable (${PROJECT} ${SOURCES})
target_link_libraries(${PROJECT} microhttpd wheels mcontrol)

install(TARGETS ${PROJECT} 
	DESTINATION ${INSTALL_PATH})
install(FILES ${STREAMER} ${GETDBM} ${SAY}
	DESTINATION ${INSTALL_PATH} 
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ 
		    GROUP_EXECUTE GROUP_READ)
install(DIRECTORY ${CONTENT} 
	DESTINATION ${CONTENT_INSTALL_PATH})