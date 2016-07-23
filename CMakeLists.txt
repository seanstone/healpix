cmake_minimum_required (VERSION 2.6)

project(ExoEarth)

# Dependencies
add_subdirectory(PulsarEngine)

# Include headers
include_directories(include)
include_directories(PulsarEngine/Pulsar/include)

# Sources
file(GLOB SOURCES "src/*.cpp")

# Options

option(DEBUG_MODE "Enable debug flags an others" OFF)

# setup some debug flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -W -pedantic -Wextra")
if(DEBUG_MODE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -g")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math")
endif()

# Output directories

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)

# Target

add_executable(exoearth ${SOURCES})

# Linking

link_directories(${CMAKE_BINARY_DIR}/lib)

if(WIN32)
	target_link_libraries(exoearth libnoise Pulsar)
else()
	target_link_libraries(exoearth noise Pulsar)
endif()

# Copy data

file(COPY glsl DESTINATION ${CMAKE_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})

# http://voices.canonical.com/jussi.pakkanen/2013/03/26/a-list-of-common-cmake-antipatterns/
