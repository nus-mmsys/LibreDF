cmake_minimum_required (VERSION 2.6)

project (tmf)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "-std=c++0x")
include_directories(.)

add_subdirectory(core)
add_subdirectory(filters)
add_subdirectory(tools)
add_subdirectory(types)
add_subdirectory(tmffactory)


add_library(tmf SHARED ${core_headers} ${core_sources}
                             ${filters_headers} ${filters_sources}
                             ${tools_headers} ${tools_sources}
                             ${types_headers} ${types_sources}
                             ${tmffactory_headers} ${tmffactory_sources} )
