project(gcomp)

aux_source_directory(. gcomp)

add_executable(${PROJECT_NAME} ${gcomp})
target_link_libraries(${PROJECT_NAME} tmfparser)
