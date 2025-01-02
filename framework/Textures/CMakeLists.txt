project(Textures)

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME}
        TextureManager.h TextureManager.cpp)


target_link_libraries(${PROJECT_NAME} PUBLIC glad stb)
