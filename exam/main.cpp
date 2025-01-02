#define GLFW_INCLUDE_NONE
#include "Exam.h"

int main(int argc, char **argv) {

    exam_App app;
    
    auto win = app.GLFWInit(app.width, app.height, "Exam");

    if (win != 0) return win;

    return app.run();

}

