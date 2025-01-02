    #version 430 core

    uniform sampler2D uTexture;
    uniform bool showTexture;

    in vec2 vs_position;
    in vec4 v_Color;

    out vec4 color;

    void main() {
        //Retrieve the texture color using vs_tcoords [8].
        color = mix(v_Color, texture(uTexture, vs_position), 0.5);
        if (!showTexture) color = v_Color;
    }