#define GLFW_INCLUDE_NONE
#include "Lab4.h"

int main(int argc, char **argv) {

    lab4_App app;
    
    auto win = app.GLFWInit(app.width, app.height, "Lab 4");

    if (win != 0) return win;

    return app.run();

}

