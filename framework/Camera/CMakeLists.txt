project(Camera)

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME}
        Camera.h Camera.cpp
        OrthographicCamera.h
        PerspectiveCamera.h)


target_link_libraries(${PROJECT_NAME} PUBLIC glm glfw)