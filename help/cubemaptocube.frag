// cubemap direction mapping
// SPDX-FileCopyrightText: © 2016, Antoine Rousseau and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
#version 120

varying vec3 texcoords;
uniform samplerCube cube_texture;

void main() {
	gl_FragColor = textureCube(cube_texture, texcoords);
}
