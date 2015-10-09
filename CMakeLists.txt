cmake_minimum_required (VERSION 2.6)

project (tmf)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
set(EXTERNAL_LIBRARIES m z avformat avcodec avutil avdevice swscale SDL)

include_directories(.)

aux_source_directory(core core_src)
aux_source_directory(filters/basic basicfilters_src)
aux_source_directory(filters/libav/tools libavtools_src)
aux_source_directory(filters/libav libavfilters_src)
aux_source_directory(filters/ehealth/tools ehealthtools_src)
aux_source_directory(filters/ehealth ehealthfilters_src)

add_library(${PROJECT_NAME} SHARED ${core_src}
                       ${basicfilters_src}
                       ${libavtools_src}
                       ${libavfilters_src}
                       ${ehealthtools_src}
                       ${ehealthfilters_src})


target_link_libraries(${PROJECT_NAME} ${EXTERNAL_LIBRARIES})

add_subdirectory(examples)