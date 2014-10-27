project(filters)


set (${PROJECT_NAME}_headers ${PROJECT_NAME}/Add2Filter.h ${PROJECT_NAME}/AdditionFilter.h ${PROJECT_NAME}/AdditionFilter.h ${PROJECT_NAME}/ImageScalerFilter.h ${PROJECT_NAME}/ImageWriterFilter.h ${PROJECT_NAME}/Multiply2Filter.h ${PROJECT_NAME}/NumberGeneratorFilter.h ${PROJECT_NAME}/VideoDecoderFilter.h ${PROJECT_NAME}/VideoDisplayFilter.h ${PROJECT_NAME}/VideoEncoderFilter.h ${PROJECT_NAME}/VideoMuxerFilter.h ${PROJECT_NAME}/VideoWriterFilter.h PARENT_SCOPE)

set (${PROJECT_NAME}_sources ${PROJECT_NAME}/VideoDisplayFilter.cpp ${PROJECT_NAME}/VideoEncoderFilter.cpp ${PROJECT_NAME}/VideoMuxerFilter.cpp ${PROJECT_NAME}/VideoWriterFilter.cpp PARENT_SCOPE)
