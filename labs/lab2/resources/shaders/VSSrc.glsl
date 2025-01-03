    #version 430 core

    layout(location = 0) in vec2 position;

    uniform vec2 selectedSquare;

    out vec4 v_Color;

    void main() {
        gl_Position = vec4(position, 0.0, 1.0); // Homogeneous coordinates 3D+1

        // The position of the pixels
        float i = position.x * 8.0f + 3.0f;
        float j = position.y * 8.0f + 3.0f;

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

        { v_Color = vec4(0.6f, 0.0f, 0.2f, 1.0f); }

        else { v_Color = vec4(0.9f, 0.8f, 1.0f, 1.0f); }

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


        // Highlight the selected square
        if ((posx == selectedSquare.x) && (posy == selectedSquare.y)) {v_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);}

    }
