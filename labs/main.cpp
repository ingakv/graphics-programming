#define GLFW_INCLUDE_NONE
#include "Lab.h"

int main(int argc, char **argv) {

    lab_App app;

    auto win = app.GLFWInit(app.width, app.height, "Lab");

    if (win != 0) return win;

    return app.run();

}

