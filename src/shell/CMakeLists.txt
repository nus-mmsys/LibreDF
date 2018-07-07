project(tmf)

include_directories(.)

aux_source_directory(analyse analyse)
aux_source_directory(deploy deploy)
aux_source_directory(parser parser)

add_executable(tmf-anlys tmf-anlys.cpp ${analyse} ${parser})

add_executable(tmf tmf-deploy.cpp ${deploy} ${parser})
target_link_libraries(tmf dataflow)

option(BUILD_RDF_MODULE "Build RDF module" OFF)

if (BUILD_RDF_MODULE)
  aux_source_directory(rdf rdf)
  add_executable(rdf-anlys ${rdf} ${analyse} ${parser})
endif()


