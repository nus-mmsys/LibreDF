cmake_minimum_required (VERSION 2.6)

project (tmf)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "-std=c++0x")
include_directories(. core filters tools types tmffactory)

aux_source_directory(core core_src)
aux_source_directory(filters filters_src)
aux_source_directory(tools tools_src)
aux_source_directory(types types_src)
aux_source_directory(tmffactory tmffactory_src)
aux_source_directory(examples examples_src)

#add_subdirectory(core)
#add_subdirectory(filters)
#add_subdirectory(tools)
#add_subdirectory(types)
#add_subdirectory(tmffactory)


add_library(tmf SHARED ${core_src}
                       ${filters_src}
                       ${tools_src}
                       ${types_src}
                       ${tmffactory_src} )


target_link_libraries(tmf avformat avcodec avutil avdevice swscale SDL)

#add_subdirectory(examples)

add_executable(example ${examples_src})

target_link_libraries(example tmf)

