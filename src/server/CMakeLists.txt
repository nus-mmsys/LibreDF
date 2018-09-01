project(tmf-server)

aux_source_directory(. src)

add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} tmfdf tmfparser)
