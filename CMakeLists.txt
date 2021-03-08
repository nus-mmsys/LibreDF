cmake_minimum_required (VERSION 2.6)

project (tmf)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)

if (APPLE)
	set(CMAKE_MACOSX_RPATH ON)
endif (APPLE)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(src)
