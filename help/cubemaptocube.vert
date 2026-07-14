// cubemap direction mapping
// SPDX-FileCopyrightText: © 2016, Antoine Rousseau and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
#version 120

varying vec3 texcoords;

void main() {
  texcoords = vec3(gl_Vertex);
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
