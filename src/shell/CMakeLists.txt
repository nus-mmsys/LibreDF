project(tdf)

aux_source_directory(. tdf_src)

add_executable(${PROJECT_NAME} ${tdf_src})

target_link_libraries(${PROJECT_NAME} df)

