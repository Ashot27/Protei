cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)

project(SkyTech)

add_subdirectory("server")
add_subdirectory("client")

enable_testing()
add_subdirectory("test")
