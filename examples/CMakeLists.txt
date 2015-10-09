cmake_minimum_required (VERSION 2.6)

project (examples)

if(${basicfilters})
  add_subdirectory(basic)
endif()

if(${libavfilters})
  add_subdirectory(libav)
endif()

if(${ehealthfilters})
  add_subdirectory(ehealth)
endif()