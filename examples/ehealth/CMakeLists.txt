cmake_minimum_required (VERSION 2.6)

project (ehealth_examples)

add_executable(sensordisplay sensordisplay.cpp)
target_link_libraries(sensordisplay tmf)

add_executable(sensorsender sensorsender.cpp)
target_link_libraries(sensorsender tmf)
