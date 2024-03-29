project(df)

set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)

find_package(PkgConfig REQUIRED)

option(BUILD_ML_ACTORS "Build machine-learning actors" ON)
option(BUILD_OPENCV_ACTORS "Build opencv actors" ON)
option(BUILD_BASIC_ACTORS "Build basic actors" ON)
option(BUILD_DELAY_ACTORS "Build delay actors" ON)
option(BUILD_VOID_ACTORS "Build void actors" ON)
option(BUILD_COMBINATORICS_ACTORS "Build combinatorics actors" ON)
option(BUILD_DESIGN_ACTORS "Build design actors" ON)
option(BUILD_SIGNAL_ACTORS "Build signal actors" ON)

include_directories(.)

if (BUILD_OPENCV_ACTORS)
  find_package(OpenCV REQUIRED)
  include_directories(${OpenCV_INCLUDE_DIRS})
endif()

aux_source_directory(core core)

if (BUILD_ML_ACTORS)
  aux_source_directory(tokens/opencv ocvtoken)
  aux_source_directory(actors/machine-learning ml)
endif()

if (BUILD_OPENCV_ACTORS)
  aux_source_directory(tokens/opencv ocvtoken)
  aux_source_directory(actors/opencv-core ocvcore)
  aux_source_directory(actors/opencv-imgproc ocvimg)
  aux_source_directory(actors/opencv-highgui ocvgui)
  aux_source_directory(actors/opencv-objdetect ocvobj)
  aux_source_directory(actors/opencv-video ocvvideo)
endif()

if (BUILD_BASIC_ACTORS)
  aux_source_directory(tokens/basic basictoken)
  aux_source_directory(actors/basic basic)
endif()

if (BUILD_DELAY_ACTORS)
  aux_source_directory(tokens/basic basictoken)
  aux_source_directory(actors/delay delay)
endif()

if (BUILD_VOID_ACTORS)
  aux_source_directory(tokens/basic basictoken)
  aux_source_directory(actors/void void)
endif()

if (BUILD_COMBINATORICS_ACTORS)
  aux_source_directory(tokens/basic basictoken)
  aux_source_directory(actors/combinatorics comb)
endif()

if (BUILD_DESIGN_ACTORS)
  aux_source_directory(tokens/basic basictoken)
  aux_source_directory(actors/design design)
endif()

if (BUILD_SIGNAL_ACTORS)
  aux_source_directory(tokens/basic basictoken)
  aux_source_directory(actors/signal signal)
endif()

add_library(${PROJECT_NAME} SHARED ${core}
				${ml} 
				${ocvcore} ${ocvimg}
				${ocvgui} ${ocvobj}
				${ocvvideo} ${ocvtoken} ${void}
				${basic} ${delay} ${basictoken} 
				${comb} ${design} ${signal}
				)

if (BUILD_OPENCV_ACTORS OR BUILD_ML_ACTORS OR BUILD_DESIGN_ACTORS)
  target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
  #add_library("opencv_dep_cudart" UNKNOWN IMPORTED)
  #set_target_properties("opencv_dep_cudart" PROPERTIES IMPORTED_LOCATION /usr/local/cuda-8.0/lib64/libcudart.so)
endif()
