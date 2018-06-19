project(rdf)

aux_source_directory(. rdf_src)

add_executable(${PROJECT_NAME} ${rdf_src})

target_link_libraries(${PROJECT_NAME} df)

