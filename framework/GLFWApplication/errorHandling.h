
// External libs includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard lib includes
#include <cstdlib>
#include <ios>
#include <iostream>

// Function Definitions

void GLFWErrorCallback(int code, const char* description)
{
    std::cerr << "GLFW Error (0x" << std::hex << code << "): " << description << "\n";
}


// -----------------------------------------------------------------------------
// MessageCallback (for debugging purposes)
// -----------------------------------------------------------------------------
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar* message, const void* userParam)
{
    std::cerr << "OpenGL Callback: " << (type == GL_DEBUG_TYPE_ERROR ? "** ERROR **" : "")
              << " Type: 0x" << type
              << ", Severity: 0x" << severity
              << ", Message: " << message << "\n";
}
