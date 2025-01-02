project(Shaders)

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME}
        ../Rendering/ShadersDataTypes.h
        Shader.h Shader.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC glm glad)
