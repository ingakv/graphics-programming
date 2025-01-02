#define GLFW_INCLUDE_NONE
#include "Lab5.h"

int main(int argc, char **argv) {

    lab5_App app;
    
    auto win = app.GLFWInit(app.width, app.height, "Lab 5");

    if (win != 0) return win;

    return app.run();

}

