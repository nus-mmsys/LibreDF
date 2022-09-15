project(dfparser)

include_directories(.)

aux_source_directory(. src)

add_library(${PROJECT_NAME} SHARED ${src})
target_link_libraries(${PROJECT_NAME} df)
target_link_libraries(${PROJECT_NAME} dfgraph)
