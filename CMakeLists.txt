cmake_minimum_required (VERSION 2.6)

project (tmf)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
set(LIBAV_LIBRARIES m z avformat avcodec avutil avdevice swscale SDL)

set(basicfilters "1")
set(libavfilters "1")
set(ehealthfilters "0")

include_directories(.)

aux_source_directory(core core_src)

if (${basicfilters})
  aux_source_directory(filters/basic basicfilters_src)
endif()

if(${libavfilters})
  aux_source_directory(filters/libav/tools libavtools_src)
  aux_source_directory(filters/libav libavfilters_src)
endif()

if(${ehealthfilters})
  aux_source_directory(filters/ehealth/tools ehealthtools_src)
  aux_source_directory(filters/ehealth ehealthfilters_src)
endif()

add_library(${PROJECT_NAME} SHARED ${core_src}
                       ${basicfilters_src}
                       ${libavtools_src}
                       ${libavfilters_src}
                       ${ehealthtools_src}
                       ${ehealthfilters_src})

if(${libavfilters})
  target_link_libraries(${PROJECT_NAME} ${LIBAV_LIBRARIES})
endif()

add_subdirectory(examples)