// SPDX-FileCopyrightText: © 2007, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
varying vec2 texcoord0;
varying vec2 texcoord1;

void main()
{

    texcoord0 = (gl_TextureMatrix[0] * gl_MultiTexCoord0).st;
    texcoord1 = (gl_TextureMatrix[1] * gl_MultiTexCoord1).st;
    gl_Position = ftransform();

}
