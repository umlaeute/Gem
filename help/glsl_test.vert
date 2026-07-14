// SPDX-FileCopyrightText: © 2024, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
// based on "OpenGL 4 Shading Language Cookbook" by David Wolff

varying vec3 Normal;
varying vec3 Position;

void main()
{
    Normal = normalize(gl_NormalMatrix * gl_Normal.xyz);
    Position = ( gl_ModelViewMatrix * vec4(gl_Vertex.xyz,1.0) ).xyz;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
