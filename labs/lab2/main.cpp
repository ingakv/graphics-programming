#define GLFW_INCLUDE_NONE
#include "Lab2.h"

int main(int argc, char **argv) {

    lab2_App app;
    
    auto win = app.GLFWInit(app.width, app.height, "Lab 2");

    if (win != 0) return win;

    return app.run();

}

