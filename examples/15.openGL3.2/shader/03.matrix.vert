// SPDX-FileCopyrightText: © 2025, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
#version 150

// from VBO
in vec3 position;
in vec3 normal;
in vec3 color;

// to geometry shader
out vec3 vertNormal;
out vec3 vertColor;

// the model view projection matrix used for transformation
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
    mat3 normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
    vertNormal = normalMatrix * normal;
    vertColor = color;
    gl_Position = (projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0)); 
}
