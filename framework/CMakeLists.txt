project(framework)

find_package(OpenGL REQUIRED)

add_subdirectory(GLFWApplication)
add_subdirectory(GeometricTools)
add_subdirectory(Shaders)
add_subdirectory(Rendering)
add_subdirectory(Camera)
add_subdirectory(Textures)

add_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC Camera GeometricTools GLFWApplication Rendering Shaders Textures
        glad glfw stb)