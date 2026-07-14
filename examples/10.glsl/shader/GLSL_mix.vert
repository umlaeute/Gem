// SPDX-FileCopyrightText: © 2010, jack/RYBN
// SPDX-FileCopyrightText: © 2010, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
varying vec2 texcoord0;

void main()
{
	texcoord0 = (gl_TextureMatrix[0]*gl_MultiTexCoord0).st;
	gl_Position = ftransform();

}
