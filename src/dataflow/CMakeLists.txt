project(df)

find_package(PkgConfig REQUIRED)
find_package(OpenCV 3.0 REQUIRED)

option(BUILD_BASIC_ACTORS "Build basic filters" ON)
option(BUILD_OPENCV_ACTORS "Build opencv filters" ON)

include_directories(.)
include_directories(${OpenCV_INCLUDE_DIRS})

aux_source_directory(core core)

if (BUILD_BASIC_ACTORS)
  aux_source_directory(actors/basic basic)
endif()

if (BUILD_OPENCV_ACTORS)
  aux_source_directory(actors/opencv-core ocvcore)
  aux_source_directory(actors/opencv-imgproc ocvimg)
  aux_source_directory(actors/opencv-highgui ocvgui)
  aux_source_directory(actors/opencv-objdetect ocvobj)
  aux_source_directory(actors/opencv-video ocvvideo)
endif()

add_library(${PROJECT_NAME} SHARED ${core} ${basic} 
				${ocvcore} ${ocvimg}
				${ocvgui} ${ocvobj}
				${ocvvideo}
				)

if (BUILD_OPENCV_ACTORS)
  target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
endif()
