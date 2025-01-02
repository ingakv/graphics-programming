#define GLFW_INCLUDE_NONE
#include "Lab1.h"

int main(int argc, char **argv) {

    lab1_App app;
    
    auto win = app.GLFWInit(app.width, app.height, "Lab 1");

    if (win != 0) return win;

    return app.run();

}

