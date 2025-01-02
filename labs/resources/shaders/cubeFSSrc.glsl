    #version 430 core

    uniform samplerCube uTexture;
    uniform vec3 u_cameraPosition;
    uniform vec3 u_lightSourcePosition;

    uniform vec3 u_ambientColor = vec3(1.0, 1.0, 1.0);
    uniform float u_ambientStrength = 1.0;

    uniform vec3 u_diffuseColor = vec3(1.0, 1.0, 1.0);
    uniform float u_diffuseStrength = 1.0;

    uniform vec3 u_specularColor = vec3(1.0, 1.0, 1.0);
    uniform float u_specularStrength = 1.0;


    in vec3 vs_position;
    in vec4 v_Color;
    in vec4 vs_normal;
    in vec4 vs_fragPosition;

    out vec4 color;

    void main() {

        // Semi-transparent color
        vec4 mix_color = mix(v_Color, texture(uTexture, vs_position), 0.7);

        vec3 lightDirection = normalize(vec3(u_lightSourcePosition - vs_fragPosition.xyz));

        // Ambient illumination
        vec4 ambient = vec4(u_ambientColor, u_ambientStrength);

        // Diffuse illumination
        vec4 diffuse = vec4(u_diffuseColor, max(dot(lightDirection, vs_normal.xyz), 0.0f) * u_diffuseStrength);


        // Specular illumination
        vec3 reflectedLight = normalize(reflect(-lightDirection, vs_normal.xyz));
        vec3 observerDirection = normalize(u_cameraPosition - vs_fragPosition.xyz);
        float specFactor = pow(max(dot(observerDirection, reflectedLight), 0.0), 12);

        vec4 specular = vec4(u_specularColor, u_specularStrength) * specFactor;


        // Compute the final color
        color = mix_color * (ambient + diffuse + specular);


    }