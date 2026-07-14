// SPDX-FileCopyrightText: © 2008, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect texture_mass;
uniform float init;

void main (void)
{
	vec2 coord = gl_TexCoord[0].st/2.;
	vec4 pos = texture2DRect(texture_mass, coord);

	gl_FragColor = pos;
}
