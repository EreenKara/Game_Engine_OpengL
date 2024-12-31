#version 330 core

in vec3 geomColor; // Geometry Shader'den gelen renk
out vec4 outColor; // Çıktı rengi

void main() {
    outColor = vec4(geomColor, 1.0); // Çizgiyi gelen renkle çiz
}