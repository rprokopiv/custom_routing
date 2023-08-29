cmake_minimum_required(VERSION 3.10)

project(common_functionality)

# Create a list of source files using file(GLOB ...)
file(GLOB COMMON_FUNCTIONALITY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create the static library
add_library(common_functionality STATIC
    ${COMMON_FUNCTIONALITY_SOURCES}
)

# Set include directories for the library
target_include_directories(common_functionality
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(common_functionality PRIVATE cxx_std_17)