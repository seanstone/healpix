cmake_minimum_required (VERSION 2.6)

project(ExoEarth)

# Include headers
include_directories(include)

# Sources
file(GLOB SOURCES "src/*.cpp")

option(DEBUG_MODE "Enable debug flags an others" OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)

link_directories(${CMAKE_BINARY_DIR}/lib)

# setup some debug flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -W -pedantic -Wextra")
if(DEBUG_MODE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -g")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O")
endif()

add_executable(ExoEarth ${SOURCES})

if(WIN32)
	target_link_libraries(ExoEarth glfw3 glew32 opengl32 libnoise)
else()
	target_link_libraries(ExoEarth glfw GLEW GL noise)
endif()

# http://voices.canonical.com/jussi.pakkanen/2013/03/26/a-list-of-common-cmake-antipatterns/
