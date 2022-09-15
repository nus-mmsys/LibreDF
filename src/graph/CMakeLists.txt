project(dfgraph)

include_directories(.)

aux_source_directory(. src)

add_library(${PROJECT_NAME} SHARED ${src})
