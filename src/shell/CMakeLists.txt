project(df)

aux_source_directory(. df_src)

add_executable(${PROJECT_NAME} ${df_src})

target_link_libraries(${PROJECT_NAME} dataflow)

