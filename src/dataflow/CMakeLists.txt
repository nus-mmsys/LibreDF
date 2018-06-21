project(dataflow)

find_package(PkgConfig REQUIRED)
find_package(OpenCV 3.0 REQUIRED)

option(BUILD_ML_ACTORS "Build machine-learning actors" ON)
option(BUILD_OPENCV_ACTORS "Build opencv actors" ON)
option(BUILD_BASIC_ACTORS "Build basic actors" ON)

include_directories(.)
include_directories(${OpenCV_INCLUDE_DIRS})

aux_source_directory(core core)

if (BUILD_ML_ACTORS)
  aux_source_directory(actors/machine-learning ml)
endif()

if (BUILD_OPENCV_ACTORS)
  aux_source_directory(actors/opencv-core ocvcore)
  aux_source_directory(actors/opencv-imgproc ocvimg)
  aux_source_directory(actors/opencv-highgui ocvgui)
  aux_source_directory(actors/opencv-objdetect ocvobj)
  aux_source_directory(actors/opencv-video ocvvideo)
endif()

if (BUILD_BASIC_ACTORS)
  aux_source_directory(actors/basic basic)
endif()

add_library(${PROJECT_NAME} SHARED ${core} ${ml} 
				${ocvcore} ${ocvimg}
				${ocvgui} ${ocvobj}
				${ocvvideo} ${basic}
				)

if (BUILD_OPENCV_ACTORS OR BUILD_ML_ACTORS)
  target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
endif()
