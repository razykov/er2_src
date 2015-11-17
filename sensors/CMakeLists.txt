cmake_minimum_required(VERSION 2.6)

set (PROJECT "Sensors")
set (TEST_ANGL_EXEC "test_ang_rot")
set (TEST_LOG_EXEC "test_log")
set (SENSORS_LIB "sensors")

project(${PROJECT})

set(LIB_INSTALL_PATH /usr/lib/)
set(HEADERS_INSTALL_PATH /usr/include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")

set(HEADERS_INSTALL hmc5883l.h)

set(SOURCES_TEST_ANGL_ROT test_angl_rot.c)
set(SOURCES_TEST_LOG test_log.c)
set(SOURCES_HMC5883L hmc5883l.c)

add_library (${SENSORS_LIB} SHARED ${SOURCES_HMC5883L})
target_link_libraries(${SENSORS_LIB} pthread m)

add_executable (${TEST_ANGL_EXEC} ${SOURCES_TEST_ANGL_ROT})
target_link_libraries(${TEST_ANGL_EXEC} ${SENSORS_LIB} wheels mcontrol)

add_executable (${TEST_LOG_EXEC} ${SOURCES_TEST_LOG})
target_link_libraries(${TEST_LOG_EXEC} ${SENSORS_LIB})

install(TARGETS ${SENSORS_LIB} DESTINATION ${LIB_INSTALL_PATH})
install(FILES ${HEADERS_INSTALL} DESTINATION ${HEADERS_INSTALL_PATH})