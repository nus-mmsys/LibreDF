project(df)

include_directories(.)

aux_source_directory(parser parser)
aux_source_directory(ui ui)

add_executable(${PROJECT_NAME} main.cpp ${parser} ${ui})

target_link_libraries(${PROJECT_NAME} dataflow)

