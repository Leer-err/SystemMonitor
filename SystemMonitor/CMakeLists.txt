cmake_minimum_required (VERSION 3.8)

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

find_package(Curses REQUIRED)

add_executable (SystemMonitor ${SOURCES} ${HEADERS})
target_link_libraries(SystemMonitor ${CURSES_LIBRARIES})
target_include_directories(SystemMonitor PUBLIC ${CURSES_INCLUDE_DIR})

set_target_properties(
	SystemMonitor PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED true
)