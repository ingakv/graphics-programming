#ifndef __GLFWAPPLICATION_H_
#define __GLFWAPPLICATION_H_

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <cstdlib>
#include <iostream>
#include <cmath>


class GLFWApplication {
public:

    static const int width = 800;
    static const int height = 800;
    GLFWwindow * window {};

    int GLFWInit(int winWidth, int winHeight, const char * winName);

    GLFWApplication() = default;

    ~GLFWApplication();

    int glLoader();

    virtual int run() = 0;

};
#endif // __GLFWAPPLICATION_H_
