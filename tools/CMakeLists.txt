project(tools)


set (${PROJECT_NAME}_headers ${PROJECT_NAME}/VideoDisplay.h ${PROJECT_NAME}/VideoEncoder.h ${PROJECT_NAME}/VideoFrameWriter.h ${PROJECT_NAME}/VideoMuxer.h ${PROJECT_NAME}/VideoReader.h ${PROJECT_NAME}/VideoScaler.h ${PROJECT_NAME}/VideoWriter.h PARENT_SCOPE)
set (${PROJECT_NAME}_sources ${PROJECT_NAME}/VideoDisplay.cpp ${PROJECT_NAME}/VideoEncoder.cpp ${PROJECT_NAME}/VideoMuxer.cpp ${PROJECT_NAME}/VideoWriter.cpp PARENT_SCOPE)
