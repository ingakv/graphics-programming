#ifndef __LAB4_H_
#define __LAB4_H_


#include "GLFWApplication/GLFWApplication.h"


class lab4_App: public GLFWApplication {
public:
    lab4_App() : GLFWApplication() {};

    int run() override;
};


#endif // __LAB4_H_
