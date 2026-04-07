#version 330 core

in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform bool uHasTexture;

void main() {
    if (uHasTexture)
        FragColor = texture(uTexture, TexCoord);
    else {
        // fallback: shade by normal so untextured models still look 3D
        vec3 n = Normal;
        float lenN = length(n);
        if (lenN > 1e-6)
            n = n / lenN;
        else
            n = vec3(0.0, 0.0, 1.0);
        n = n * 0.5 + 0.5;
        FragColor = vec4(n, 1.0);
    }
}