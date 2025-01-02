#ifndef __LAB5_H_
#define __LAB5_H_


#include "GLFWApplication/GLFWApplication.h"


class lab5_App: public GLFWApplication {
public:
    lab5_App() : GLFWApplication() {};

    int run() override;
};


#endif // __LAB5_H_
