#ifndef __LAB1_H_
#define __LAB1_H_


#include "GLFWApplication/GLFWApplication.h"


class lab1_App: public GLFWApplication {
public:
    lab1_App() : GLFWApplication() {};

    int run() override;
};


#endif // __LAB1_H_
