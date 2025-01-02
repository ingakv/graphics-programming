    #version 430 core

    layout(location = 0) in vec3 position;

    uniform mat4 u_TransformationMat;


    out vec4 v_Color;

    void main() {
        // Combine all of the matrices together to get the correct coordinates
        gl_Position = u_TransformationMat * vec4(position, 1.0f);

        v_Color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
