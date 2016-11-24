cmake_minimum_required (VERSION 2.6)

project(ExoEarth)

option(DEBUG_MODE off)
option(EMSCRIPTEN off)

# Check if Emscripten
if(${CMAKE_SYSTEM} STREQUAL Emscripten-1)
	option(EMSCRIPTEN on)
	option(ENABLE_ASSIMP off)
endif()

# Dependencies
add_subdirectory(PulsarEngine)

# Include headers
include_directories(include)
include_directories(lib/libnoise/src)
include_directories(PulsarEngine/Pulsar/include)
include_directories(PulsarEngine/lib)
include_directories(PulsarEngine/lib/glm)

# Sources
file(GLOB SOURCES "src/*.cpp")

# Output directories

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)

# Compile flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -W -pedantic -Wextra")
if(DEBUG_MODE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -g")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math")
endif()
if(EMSCRIPTEN)
	# see https://github.com/kripken/emscripten/blob/master/src/settings.js
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --llvm-lto 0 -s NO_EXIT_RUNTIME=1 -s USE_SDL=2 -s USE_LIBPNG=1 -s USE_ZLIB=1")
	if(NOT DEBUG_MODE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --closure 1")
	endif()
endif()

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

# Copy data
if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/data@/data")
else()
    file(COPY data DESTINATION ${CMAKE_BINARY_DIR})
endif()

# http://voices.canonical.com/jussi.pakkanen/2013/03/26/a-list-of-common-cmake-antipatterns/
