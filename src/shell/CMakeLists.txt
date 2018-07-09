project(tmf)

include_directories(.)

aux_source_directory(analyse analyse)
aux_source_directory(deploy deploy)
aux_source_directory(parser parser)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp ${analyse} ${parser})

add_executable(${PROJECT_NAME}-deploy ${PROJECT_NAME}-deploy.cpp ${deploy} ${parser})
target_link_libraries(${PROJECT_NAME}-deploy dataflow)

option(BUILD_RDF_MODULE "Build RDF module" OFF)

if (BUILD_RDF_MODULE)
  aux_source_directory(rdf rdf)
  add_executable(rdf ${rdf} ${analyse} ${parser})
endif()


