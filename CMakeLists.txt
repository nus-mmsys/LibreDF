cmake_minimum_required (VERSION 2.6)

project (tmf)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
set(LIBAV_LIBRARIES m z avformat avcodec avutil avdevice swscale SDL)

option(BUILD_BASIC_FILTERS "Build basic filters" ON)
option(BUILD_LIBAV_FILTERS "Build libav filters" ON)
option(BUILD_EHEALTH_FILTERS "Build ehealth filters" OFF)

include_directories(.)

aux_source_directory(core core_src)
aux_source_directory(tools tools_src)

if (BUILD_BASIC_FILTERS)
  aux_source_directory(filters/basic basicfilters_src)
endif()

if(BUILD_LIBAV_FILTERS)
  aux_source_directory(filters/libav/tools libavtools_src)
  aux_source_directory(filters/libav libavfilters_src)
endif()

if(BUILD_EHEALTH_FILTERS)
  aux_source_directory(filters/ehealth/tools ehealthtools_src)
  aux_source_directory(filters/ehealth ehealthfilters_src)
endif()

add_library(${PROJECT_NAME} SHARED ${core_src}
                       ${tools_src}
                       ${basicfilters_src}
                       ${libavtools_src}
                       ${libavfilters_src}
                       ${ehealthtools_src}
                       ${ehealthfilters_src})

if(BUILD_LIBAV_FILTERS)
  target_link_libraries(${PROJECT_NAME} ${LIBAV_LIBRARIES})
endif()

if(BUILD_EHEALTH_FILTERS)
  target_link_libraries(${PROJECT_NAME} ehealthsensor curl)
endif()


add_subdirectory(examples)