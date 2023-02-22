cmake_minimum_required (VERSION 3.20)

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

list (REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)

find_package(Slint REQUIRED)

add_executable (SystemMonitor ${SOURCES} ${HEADERS})

target_link_libraries(SystemMonitor PRIVATE Slint::Slint ${CURSES_LIBRARIES} pthread) 
target_include_directories(SystemMonitor PRIVATE ${CURSES_INCLUDE_DIR} ${SLINT_INCLUDE_DIR})
slint_target_sources(SystemMonitor ui.slint)

set_target_properties(
	SystemMonitor PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED true
)

list (REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
list (APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)

add_executable(Test ${SOURCES} ${HEADERS})
target_link_libraries(Test PRIVATE Slint::Slint ${CURSES_LIBRARIES} pthread)
target_include_directories(Test PRIVATE ${CURSES_INCLUDE_DIR} ${SLINT_INCLUDE_DIR})
slint_target_sources(Test ui.slint)

set_target_properties(
	Test PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED true
)

enable_testing()

add_test(
  	NAME pid_test
  	COMMAND $<TARGET_FILE:Test> pid
)
add_test(
  	NAME poll_test
  	COMMAND $<TARGET_FILE:Test> poll
)
add_test(
	NAME signal_test
	COMMAND $<TARGET_FILE:Test> signal
)