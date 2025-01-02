# Set the minimum required version of CMake that the project can use.
# This ensures that the version of CMake used to build the project has
# all the features that are required.
cmake_minimum_required(VERSION 3.15)

project(Lab1)

# Search for the OpenGL package on the system. The REQUIRED argument
# ensures that if OpenGL is not found, CMake will produce an error and
# stop processing. If found, this command sets up various variables
# and imported targets related to OpenGL that can be used later.
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp Lab1.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC framework)
