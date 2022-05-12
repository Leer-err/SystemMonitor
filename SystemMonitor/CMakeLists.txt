cmake_minimum_required (VERSION 3.8)

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

add_executable (SystemMonitor ${SOURCES} ${HEADERS})
target_link_libraries(SystemMonitor "pthread")

set_target_properties(
	SystemMonitor PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED true
)