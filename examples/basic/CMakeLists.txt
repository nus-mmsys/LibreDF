cmake_minimum_required (VERSION 2.6)

project (basic_examples)

add_executable(example00 example00.cpp)
target_link_libraries(example00 tmf)

add_executable(example01 example01.cpp)
target_link_libraries(example01 tmf)

add_executable(example02 example02.cpp)
target_link_libraries(example02 tmf)
    
add_executable(example03 example03.cpp)
target_link_libraries(example03 tmf)

add_executable(example04 example04.cpp)
target_link_libraries(example04 tmf)