project(rdf)

aux_source_directory(. rdfsrc)

add_executable(${PROJECT_NAME} ${rdfsrc} ../analyse/analyse.cpp)
target_link_libraries(${PROJECT_NAME} dataflow tmfparser)
