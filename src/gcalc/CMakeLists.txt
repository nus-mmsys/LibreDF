project(gcalc)

aux_source_directory(. gcalc)

add_executable(${PROJECT_NAME} ${gcalc})
target_link_libraries(${PROJECT_NAME} tmfparser)
