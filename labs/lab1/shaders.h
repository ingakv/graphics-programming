#ifndef __SHADERS_H_
#define __SHADERS_H_



/* === SHADER SETUP ===
 * Shaders are programs that run on the GPU.
 * We define, compile, and link a basic vertex and fragment shader.
 * The vertex shader processes each vertex and the fragment shader processes each pixel.
 */


#include <string>

const std::string vertex = R"(

    #version 430 core

    layout(location = 0) in vec2 position;
    //layout(location = 1) in vec3 a_Color;

    //out vec4 v_Color;

    void main() {
      gl_Position = vec4(position, 0.0, 1.0); // Homogeneous coordinates 3D+1
        //v_Color = vec4(a_Color,1.0f);
    }

)";


const std::string fragment = R"(
    #version 430 core

    uniform vec4 u_Color;

    out vec4 color;

    void main() {
        color = u_Color;
    }
)";



#endif // __SHADERS_H_
