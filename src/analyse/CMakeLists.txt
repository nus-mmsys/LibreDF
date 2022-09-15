project(df-analyse)

aux_source_directory(. analyse)

add_executable(${PROJECT_NAME} ${analyse})
target_link_libraries(${PROJECT_NAME} df dfparser)
