cmake_minimum_required (VERSION 2.6)

project (examples)

if(BUILD_BASIC_FILTERS)
  add_subdirectory(basic)
endif()

if(BUILD_LIBAV_FILTERS)
  add_subdirectory(libav)
endif()

if(BUILD_EHEALTH_FILTERS)
  add_subdirectory(ehealth)
endif()