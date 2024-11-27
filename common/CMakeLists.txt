cmake_minimum_required(VERSION 3.10)

project(common_functionality)

# Create a list of source files
file(GLOB COMMON_FUNCTIONALITY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create the static library
add_library(common_functionality STATIC ${COMMON_FUNCTIONALITY_SOURCES})

# Set include directories for the library
target_include_directories(common_functionality PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


# Link with the
target_link_libraries(common_functionality PRIVATE boost)
# Ensure C++17
set_target_properties(common_functionality PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)
