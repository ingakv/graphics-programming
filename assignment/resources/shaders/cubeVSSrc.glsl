    #version 430 core

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal; // Declare the normal vertex attribute

    uniform mat4 u_TransformationMat;

    // The currently selected square which should be highlighted
    uniform vec2 selectedSquare;

    // The selected cube which should be highlighted
    uniform vec2 selectedCube;

    // The cube's position on the grid
    uniform vec2 cubePosition;

    // If the current cube is part of the red team (or the blue team)
    uniform bool redTeam;

    // Output variable to pass texture coordinates to the fragment shader
    out vec3 vs_position;
    out vec4 v_Color;
    out vec4 vs_normal;
    out vec4 vs_fragPosition;

    void main() {

        // Change the coordinates of the cube to align it onto the right spot on the board
        vec3 pos = position;
        vec2 cubePos = cubePosition;

        // Lifts the cube up on top of the chess board
        pos[2] += 0.6f;

        // Center the cubes
        float squareSize = 1.425f;

        pos[0] += (cubePos.x - 3.5f) * squareSize;
        pos[1] += (cubePos.y - 3.5f) * squareSize;


        // Combine all of the matrices together to get the correct coordinates
        gl_Position = u_TransformationMat * vec4(pos, 1.0f);
        vs_position = position;

        vs_normal = normalize(u_TransformationMat * vec4(normal, 1.0));
        vs_fragPosition = u_TransformationMat * vec4(position, 1.0);

        if (redTeam) v_Color = vec4(0.7f, 0.0f, 0.0f, 1.0f);
        else v_Color = vec4(0.0f, 0.0f, 0.7f, 1.0f);


        // Highlight the cube that is on the selected square
        if ((cubePos.x == selectedSquare.x) && (cubePos.y == selectedSquare.y)) { v_Color = vec4(0.7f, 0.0f, 0.7f, 1.0f); }

        // Highlight the selected cube
        if ((cubePos.x == selectedCube.x) && (cubePos.y == selectedCube.y)) { v_Color = vec4(1.0f, 0.1f, 0.7f, 1.0f); }


    }