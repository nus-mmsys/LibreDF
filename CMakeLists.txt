cmake_minimum_required (VERSION 2.6)

project (tmf)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
set(EXTERNAL_LIBRARIES m z avformat avcodec avutil avdevice swscale SDL)

include_directories(. core filters tools types tmffactory)

aux_source_directory(core core_src)
aux_source_directory(filters filters_src)
aux_source_directory(tools tools_src)
aux_source_directory(types types_src)
aux_source_directory(tmffactory tmffactory_src)

add_library(${PROJECT_NAME} SHARED ${core_src}
                       ${filters_src}
                       ${tools_src}
                       ${types_src}
                       ${tmffactory_src} )


target_link_libraries(${PROJECT_NAME} ${EXTERNAL_LIBRARIES})

add_subdirectory(examples)

