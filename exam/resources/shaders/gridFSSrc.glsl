    #version 430 core

    uniform sampler2D uTexture;

    // Whether or not the textures should be drawn
    uniform bool showTexture;

    // The camera and light source position
    uniform vec3 u_cameraPosition;
    uniform vec3 u_lightSourcePosition;

    // The illumination colors and strengths
    uniform vec3 u_ambientColor = vec3(1.0, 1.0, 1.0);
    uniform float u_ambientStrength = 1.0;

    uniform vec3 u_diffuseColor = vec3(1.0, 1.0, 1.0);
    uniform float u_diffuseStrength = 1.0;

    uniform vec3 u_specularColor = vec3(1.0, 1.0, 1.0);
    uniform float u_specularStrength = 1.0;
    uniform int u_shininessFactor = 12;


    in vec2 vs_position;
    in vec3 v_Color;
    in vec3 vs_fragPosition;

    out vec4 color;

    void main() {

        vec3 normal = {0.0f, 0.0f, 1.0f};

        vec3 lightDirection = normalize(vec3(u_lightSourcePosition - vs_fragPosition));

        // Ambient illumination
        vec3 ambient = u_ambientColor * u_ambientStrength;

        // Diffuse illumination
        float diffuseStrength = max(dot(lightDirection, normal), 0.0f) * u_diffuseStrength;

        vec3 diffuse = u_diffuseColor * diffuseStrength;


        // Specular illumination
        vec3 reflectedLight = normalize(reflect(-lightDirection, normal));
        vec3 observerDirection = normalize(u_cameraPosition - vs_fragPosition);
        float specFactor = pow(max(dot(observerDirection, reflectedLight), 0.0), u_shininessFactor);

        vec3 specular = u_specularColor * u_specularStrength * specFactor;


        // Compute the final color
        vec3 textureColor = mix(v_Color, texture(uTexture, vs_position).xyz, 0.5);


        // Add textures when the T key is pressed
        if (showTexture) color =  vec4(textureColor * (u_ambientStrength + diffuseStrength + specular), 1.0);
        else color = vec4(v_Color * (ambient + diffuse + specular), 1.0);

    }