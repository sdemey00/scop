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
        vec3 n = normalize(Normal) * 0.5 + 0.5;
        FragColor = vec4(n, 1.0);
    }
}