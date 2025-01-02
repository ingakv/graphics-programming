    #version 430 core

    uniform samplerCube uTexture;

    in vec3 vs_position;
    in vec4 v_Color;

    out vec4 color;

    void main() {

        vec4 mix_color = mix(v_Color, texture(uTexture, vs_position), 0.7);

        // Semi-transparent color
        color = vec4(mix_color.rgb, 0.5);

    }