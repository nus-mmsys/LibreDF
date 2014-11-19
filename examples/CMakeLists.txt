cmake_minimum_required (VERSION 2.6)

project (examples)

add_executable(example00 example00.cpp)
target_link_libraries(example00 tmf)

add_executable(example01 example01.cpp)
target_link_libraries(example01 tmf)

add_executable(example02 example02.cpp)
target_link_libraries(example02 tmf)
    
add_executable(example1 example1.cpp)
target_link_libraries(example1 tmf)

add_executable(example2 example2.cpp)
target_link_libraries(example2 tmf)

add_executable(example3 example3.cpp)
target_link_libraries(example3 tmf)

add_executable(example4 example4.cpp)
target_link_libraries(example4 tmf)

add_executable(example5 example5.cpp)
target_link_libraries(example5 tmf)

add_executable(example6 example6.cpp)
target_link_libraries(example6 tmf)
