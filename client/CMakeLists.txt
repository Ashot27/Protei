cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)

project(client)

set(HEADER_FILES 
    "main.h" 
    "Client.h" 
    "Client_status.h" 
)

add_executable (Client 
    "main.cpp" 
    "Client.cpp" 
)