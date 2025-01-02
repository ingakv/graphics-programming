#define GLFW_INCLUDE_NONE
#include "Assigment.h"

int main(int argc, char **argv) {

    assignment_App app;
    
    auto win = app.GLFWInit(app.width, app.height, "Assignment");

    if (win != 0) return win;

    return app.run();

}

