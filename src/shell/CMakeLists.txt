project(tmf)

include_directories(.)

option(BUILD_RDF_MODULE "Build RDF module" OFF)

if (BUILD_RDF_MODULE)
  aux_source_directory(rdf module)
else()
  aux_source_directory(. module)
endif()

aux_source_directory(parser parser)
aux_source_directory(ui ui)

add_executable(${PROJECT_NAME} ${module} ${parser} ${ui})

target_link_libraries(${PROJECT_NAME} dataflow)

