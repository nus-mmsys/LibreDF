cmake_minimum_required (VERSION 2.6)

project (ehealth_examples)

add_executable(ehealth-monitor ehealth-monitor.cpp)
target_link_libraries(ehealth-monitor tmf)

add_executable(ehealth-client ehealth-client.cpp)
target_link_libraries(ehealth-client tmf)

add_executable(sensorsender sensorsender.cpp)
target_link_libraries(sensorsender tmf)
