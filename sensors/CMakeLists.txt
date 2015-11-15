cmake_minimum_required(VERSION 2.6)

set (PROJECT "Sensors")
set (TEST_EXEC "test")
set (SENSORS_LIB "sensors")

project(${PROJECT})

set(LIB_INSTALL_PATH /usr/lib/)
set(HEADERS_INSTALL_PATH /usr/include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")

set(HEADERS_INSTALL hmc5883l.h)

set(SOURCES_TEST test.c)
set(SOURCES_HMC5883L hmc5883l.c)

add_library (${SENSORS_LIB} SHARED ${SOURCES_HMC5883L})
target_link_libraries(${SENSORS_LIB} pthread)

add_executable (${TEST_EXEC} ${SOURCES_TEST})
target_link_libraries(${TEST_EXEC} ${SENSORS_LIB})


install(TARGETS ${SENSORS_LIB} DESTINATION ${LIB_INSTALL_PATH})
install(FILES ${HEADERS_INSTALL} DESTINATION ${HEADERS_INSTALL_PATH})