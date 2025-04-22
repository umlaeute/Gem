#version 150
// Cyrille Henry 2025

// from VBO
in float positionX;
in float positionY;

// to geometry shader
out vec3 vertPosition;

void main() {
    vertPosition = vec3(positionX, positionY, 0.0);
    gl_Position = vec4(positionX, positionY, 0.0, 1.0);
}
