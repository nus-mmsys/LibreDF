project(core)


set (${PROJECT_NAME}_headers ${PROJECT_NAME}/Buffer.h ${PROJECT_NAME}/Filter.h ${PROJECT_NAME}/Message.h ${PROJECT_NAME}/Pipeline.h ${PROJECT_NAME}/Port.h PARENT_SCOPE)
set (${PROJECT_NAME}_sources ${PROJECT_NAME}/Filter.cpp ${PROJECT_NAME}/Pipeline.cpp PARENT_SCOPE)
