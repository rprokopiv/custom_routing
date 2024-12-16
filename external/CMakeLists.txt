cmake_minimum_required(VERSION 3.10)

project(external)

# Add rapidjson as an interface library
add_library(rapidjson INTERFACE)

# Set include directories for rapidjson
target_include_directories(rapidjson INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include)

# Add Boost as an internal library
add_library(boost INTERFACE)

# Set include directories for Boost
target_include_directories(boost INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/boost)