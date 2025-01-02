#ifndef __EXAM_H_
#define __EXAM_H_


#include "GLFWApplication/GLFWApplication.h"


class exam_App: public GLFWApplication {
public:

    static const int width = 1024;
    static const int height = width;

    exam_App() : GLFWApplication() {};

    int run() override;
};


#endif // __EXAM_H_
