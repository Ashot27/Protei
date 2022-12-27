cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)

project(test)

set(HEADER_FILES 
    "String_parser.h"
)
add_executable (Test 
    "main.cpp" 
    "String_parser_test.cpp"
)


target_link_libraries(Test gtest)
#add_definitions(-w) #hide warnings

