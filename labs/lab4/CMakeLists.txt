# Set the minimum required version of CMake that the project can use.
# This ensures that the version of CMake used to build the project has
# all the features that are required.
cmake_minimum_required(VERSION 3.15)

project(Lab4)

# Search for the OpenGL package on the system. The REQUIRED argument
# ensures that if OpenGL is not found, CMake will produce an error and
# stop processing. If found, this command sets up various variables
# and imported targets related to OpenGL that can be used later.
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp Lab4.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE framework)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/../resources/textures/gatos.png
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../resources/textures/gatos.png)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        STB_IMAGE_IMPLEMENTATION
        STB_IMAGE_STATIC
        TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../resources/textures/"
        # Define a preprocessor macro for the STB image library
        STB_IMAGE_IMPLEMENTATION
        STB_IMAGE_STATIC)
