#define GLFW_INCLUDE_NONE
#include "Lab3.h"

int main(int argc, char **argv) {

    lab3_App app;

    auto win = app.GLFWInit(app.width, app.height, "Lab 3");

    if (win != 0) return win;

    return app.run();

}

