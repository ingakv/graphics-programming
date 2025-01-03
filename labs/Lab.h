#ifndef __LAB_H_
#define __LAB_H_


#include "GLFWApplication/GLFWApplication.h"


class lab_App: public GLFWApplication {
public:
    lab_App() : GLFWApplication() {};

    int run() override;
};


#endif // __LAB_H_
