project(tmf)

set(TMF ${PROJECT_NAME})
set(TMF_DEPLOY ${PROJECT_NAME}-deploy)
set(TMF_SERVER ${PROJECT_NAME}-server)

include_directories(.)

aux_source_directory(analyse analyse)
aux_source_directory(deploy deploy)
aux_source_directory(server server)
aux_source_directory(parser parser)

add_executable(${TMF} ${TMF}.cpp ${analyse} ${parser})
target_link_libraries(${TMF} dataflow)

add_executable(${TMF_DEPLOY} ${TMF_DEPLOY}.cpp ${deploy} ${parser})
target_link_libraries(${TMF_DEPLOY} dataflow)

add_executable(${TMF_SERVER} ${TMF_SERVER}.cpp ${server} ${parser})
target_link_libraries(${TMF_SERVER} dataflow)

option(BUILD_RDF_MODULE "Build RDF module" OFF)

if (BUILD_RDF_MODULE)

  set(RDF rdf)
  aux_source_directory(rdf rdf)
  add_executable(${RDF} ${rdf} ${analyse} ${parser})
  target_link_libraries(${RDF} dataflow)
endif()
