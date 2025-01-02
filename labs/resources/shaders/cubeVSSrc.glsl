    #version 430 core

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal; // Declare the normal vertex attribute

    uniform mat4 u_TransformationMat;

    // Output variable to pass position coordinates to the fragment shader
    out vec3 vs_position;
    out vec4 v_Color;
    out vec4 vs_normal;
    out vec4 vs_fragPosition;

    void main() {
        // Combine all of the matrices together to get the correct coordinates
        gl_Position = u_TransformationMat * vec4(position, 1.0f);
        vs_position = position;

        vs_normal = normalize(u_TransformationMat * vec4(normal, 1.0));
        vs_fragPosition = u_TransformationMat * vec4(position, 1.0);

        // The position of the pixels
        float i = sin(position.x)+0.5f;
        v_Color = vec4(0.5f, 0.0f, i, 1.0f);
    }