    #version 430 core

    layout(location = 0) in vec2 position;


    uniform float boardSize = 10;

    uniform mat4 u_TransformationMat;

    // Output variable to pass texture coordinates to the fragment shader
    out vec2 vs_position;
    out vec3 v_Color;
    out vec3 vs_fragPosition;


    void main() {

        // Combine all of the matrices together to get the correct coordinates
        gl_Position = u_TransformationMat * vec4(position, 0.0, 1.0);
        vs_position = position;

        vs_fragPosition = gl_Position.xyz;

        // The position of the pixels
        float i = position.x * boardSize + 3.0f;
        float j = position.y * boardSize + 3.0f;

        float k = i;
        float l = j;

        // The index of the square on the board
        float posx = 0;
        float posy = 0;

        bool alternate = true;

        // Every other tile will have the same color
        while(i > 2.0f) {i -= 2;};
        while(j > 2.0f) {j -= 2;};

        // Alternates between whether white or black is the first color of the row
        if (j <= 1.0f && j >= 0.0f) { alternate = !alternate; }

        if ((alternate && (i <= 1.0f && i >= 0.0f)) ||
            ((!alternate) && (i >= 1.0f || i <= 0.0f)))

        { v_Color = vec3(0.6f, 0.0f, 0.2f); }

        else { v_Color = vec3(0.9f, 0.8f, 1.0f); }

        // Find the index of the row the current square
        while(k >= -1.0f) {
            if (k > 0.0f) { posx++; }
            k--;
        };

        // Find the index of the column the current square
        while(l >= -1.0f) {
            if (l > 0.0f) { posy++; }
            l--;
        };

    }