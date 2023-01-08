cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)

project(server)

set(HEADER_FILES 
    "main.h" 
    "Server.h" 
    "Server_status.h" 
    "../key_parser.h"
)

add_executable (Server 
    "main.cpp" 
    "Server.cpp"
    "../key_parser.cpp"
)