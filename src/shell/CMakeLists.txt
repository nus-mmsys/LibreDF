project(df)

include_directories(.)

option(BUILD_RDF_MODULE "Build RDF module" ON)

if (BUILD_RDF_MODULE)
  aux_source_directory(rdf rdf)
endif()

aux_source_directory(parser parser)
aux_source_directory(ui ui)

add_executable(${PROJECT_NAME} main.cpp ${parser} ${ui} ${rdf})

target_link_libraries(${PROJECT_NAME} dataflow)

