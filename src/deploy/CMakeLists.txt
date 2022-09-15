project(df-deploy)

aux_source_directory(. src)

add_executable(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} df dfparser)
