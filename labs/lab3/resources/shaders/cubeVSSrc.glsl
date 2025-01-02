    #version 430 core

    layout(location = 0) in vec3 position;

    uniform mat4 u_TransformationMat;


    out vec4 v_Color;

    void main() {

        // Combine all of the matrices together to get the correct coordinates
        gl_Position = u_TransformationMat * vec4(position, 1.0f);

        // The position of the pixels
        float i = sin(position.x)+0.5f;
        v_Color = vec4(0.5f, 0.0f, i, 1.0f);

    }