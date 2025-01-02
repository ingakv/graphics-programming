    #version 430 core

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal; // Declare the normal vertex attribute

    uniform mat4 u_TransformationMat;

    // The cubes' positions around the grid
    uniform vec2 cubePosition;

    // What object type the cube is representing
    uniform int objectType = 0;

    uniform float boardSize = 10;

    // Output variable to pass texture coordinates to the fragment shader
    out vec3 vs_position;
    out vec3 v_Color;
    out vec3 vs_normal;
    out vec3 vs_fragPosition;

    out float vs_objectType;

    void main() {

        vec3 pos = position;

        // Lifts the cube up on top of the chess board
        pos[2] += 0.5f;

        float scale = 1.01f;



        if(objectType == 1) scale = 1.425f;

        // Center the cubes and change their coordinates in order to align it onto the right spot on the board
        pos[0] += (cubePosition.x - (0.5f * (boardSize-1))) * scale;
        pos[1] += (cubePosition.y - (0.5f * (boardSize-1))) * scale;



        // Combine all of the matrices together to get the correct coordinates
        gl_Position = u_TransformationMat * vec4(pos, 1.0f);
        vs_position = position;

        vs_normal = normalize(u_TransformationMat * vec4(normal, 1.0)).xyz;
        vs_fragPosition = gl_Position.xyz;

        vs_objectType = objectType;



        // Apply the color

        // Walls = 0
        // Player = 1
        // Box = 2
        // Target = 3
        // Box on target = 4

        switch(objectType) {
            case 1: v_Color = vec3(1.0f, 0.4f, 0.7f); break;
            case 2: v_Color = vec3(1.0f, 1.0f, 0.4f); break;
            case 3: case 4: v_Color = vec3(0.4f, 1.0f, 0.7f); break;
            default: v_Color = vec3(0.0f, 0.13f, 0.66f); break;
        };

    }