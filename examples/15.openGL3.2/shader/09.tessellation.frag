// SPDX-FileCopyrightText: © 2025, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
#version 400

in VS_OUT {
    vec3 pos;
    vec3 norm;
    vec3 color;
} fs_in;

out vec4 FragColor;

void main() {
// interpolation of the color computed at each vertices, after subdivision by the teselarisation shader
    FragColor = vec4(fs_in.color, 1.0);
}

