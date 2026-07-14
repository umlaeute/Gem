// SPDX-FileCopyrightText: © 2024, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
// based on "OpenGL 4 Shading Language Cookbook" by David Wolff
#version 460


// This is the fragment shader : it is executed for every pixel to display

in vec3 Color_to_frag;
// the variable commint from the VBO

out vec4 FragColor;
// The only output of this shader : the color of the pixel

void main() {
	FragColor = vec4(Color_to_frag, 1.0);
	// the "Color_to_frag" variable is already interpolated between the 2 shaders.
}
