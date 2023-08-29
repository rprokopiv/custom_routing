cmake_minimum_required(VERSION 3.10)


# Create a list of source files using file(GLOB ...)
file(GLOB TASK1_DISTANCE_CALCULATOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Add the executable
add_executable(task1_distance_calculator
    ${TASK1_DISTANCE_CALCULATOR_SOURCES}
)

find_package(Boost REQUIRED COMPONENTS program_options)

# Link the common_functionality library to the executable
target_link_libraries(task1_distance_calculator PRIVATE common_functionality)
target_link_libraries(task1_distance_calculator PRIVATE Boost::program_options)

# Set include directories for the library
target_include_directories(task1_distance_calculator
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set include directories for the executable
#target_include_directories(common_functionality
#    PRIVATE
#        ${CMAKE_SOURCE_DIR}/../common/include
#)

target_compile_features(task1_distance_calculator PRIVATE cxx_std_17)
