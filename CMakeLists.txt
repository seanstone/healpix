cmake_minimum_required (VERSION 2.6)

project(Earth)

# Options
option(DEBUG_MODE off)
option(EMSCRIPTEN off)

# Check if Emscripten
if(${CMAKE_SYSTEM} STREQUAL Emscripten-1)
	option(EMSCRIPTEN on)
endif()

# Include headers
include_directories(include)
include_directories(PulsarEngine/Pulsar/include)
include_directories(PulsarEngine/lib)
include_directories(lib/libnoise/src)
include_directories(lib/libnoise/noiseutils)

# Compile flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp -Wall")
if(DEBUG_MODE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -g")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math")
endif()
if(EMSCRIPTEN)
	# see https://github.com/kripken/emscripten/blob/master/src/settings.js
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --llvm-lto 0  -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=1 -s USE_SDL=2 -s USE_LIBPNG=1 -s USE_ZLIB=1")
	if(NOT DEBUG_MODE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --closure 1")
	endif()
endif()

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
if(EMSCRIPTEN)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)
else()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

# Dependencies
add_subdirectory(PulsarEngine)
if(EMSCRIPTEN)
	add_subdirectory(lib/libnoise)
endif()

# Sources
file(GLOB SOURCES "src/*.cpp")

# Target
add_executable(earth ${SOURCES})

# Linking
link_directories(${CMAKE_BINARY_DIR}/lib)
if(UNIX)
	target_link_libraries(earth noise Pulsar)
elseif(WIN32)
	target_link_libraries(earth libnoise Pulsar)
endif()

# Copy data
ADD_CUSTOM_TARGET(link_target ALL COMMAND ${CMAKE_COMMAND} -E
	create_symlink ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data)
if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/data@/data")
endif()

# http://voices.canonical.com/jussi.pakkanen/2013/03/26/a-list-of-common-cmake-antipatterns/
