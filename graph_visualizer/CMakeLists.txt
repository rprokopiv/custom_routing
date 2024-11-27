cmake_minimum_required(VERSION 3.10)

project(graph_visualizer)

# Collect all source files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Add the executable
add_executable(graph_visualizer ${SOURCES})

target_include_directories(graph_visualizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link with the common_functionality and rapidjson libraries
target_link_libraries(graph_visualizer PRIVATE common_functionality rapidjson boost)

# Set include directories for graph_visualizer
target_include_directories(graph_visualizer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/common/include
)

# Ensure C++17
set_target_properties(graph_visualizer PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)
