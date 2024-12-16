cmake_minimum_required(VERSION 3.10)

project(custom_routing)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external)
add_subdirectory(common)
add_subdirectory(graph_visualizer)
