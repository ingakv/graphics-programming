#include "GLFWApplication.h"


GLFWApplication::~GLFWApplication() {
    // Termination code (SECTION 6)
    glDisableVertexAttribArray(0);

    glfwTerminate();
}


int GLFWApplication::glLoader() {
    // OpenGL initialization code (SECTION 3)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    return 0;
}



// GLFW Window Initiation
int GLFWApplication::GLFWInit(int winWidth, int winHeight, const char * windowName) {

    // GLFW initialization code (SECTION 2)
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(winWidth, winHeight, windowName, nullptr, nullptr);


    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }


    glfwMakeContextCurrent(window);

    return this->glLoader();

}


