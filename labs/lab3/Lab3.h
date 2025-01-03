#ifndef __LAB3_H_
#define __LAB3_H_


#include "GLFWApplication/GLFWApplication.h"


class lab3_App: public GLFWApplication {
public:
    lab3_App() : GLFWApplication() {};

    int run() override;
};


#endif // __LAB3_H_
