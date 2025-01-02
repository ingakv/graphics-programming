#ifndef __ASSIGNMENT_H_
#define __ASSIGNMENT_H_


#include "GLFWApplication/GLFWApplication.h"


class assignment_App: public GLFWApplication {
public:

    static const int width = 800 * 1.5;
    static const int height = 600 * 1.5;

    assignment_App() : GLFWApplication() {};

    int run() override;
};


#endif // __ASSIGNMENT_H_
