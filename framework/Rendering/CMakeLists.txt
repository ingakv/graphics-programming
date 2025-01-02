project(Rendering)

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME}
    VertexBuffer.h VertexBuffer.cpp
    IndexBuffer.h IndexBuffer.cpp
    BufferLayout.h
    VertexArray.h VertexArray.cpp
    RenderCommands.h)

target_link_libraries(${PROJECT_NAME} PUBLIC glad stb)
