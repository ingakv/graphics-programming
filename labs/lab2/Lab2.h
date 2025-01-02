#ifndef __LAB2_H_
#define __LAB2_H_


#include "GLFWApplication/GLFWApplication.h"


class lab2_App: public GLFWApplication {
public:
    lab2_App() : GLFWApplication() {};

    int run() override;
};


#endif // __LAB2_H_
