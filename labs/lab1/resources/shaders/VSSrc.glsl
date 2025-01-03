
#version 430 core

layout(location = 0) in vec2 position;
//layout(location = 1) in vec3 a_Color;

//out vec4 v_Color;

void main() {
    gl_Position = vec4(position, 0.0, 1.0); // Homogeneous coordinates 3D+1
    //v_Color = vec4(a_Color,1.0f);
}
