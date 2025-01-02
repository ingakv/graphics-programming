project(GLFWApplication)

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME}
        GLFWApplication.h GLFWApplication.cpp
        errorHandling.h)

target_link_libraries(${PROJECT_NAME} PUBLIC glad glfw)
