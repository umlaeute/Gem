// SPDX-FileCopyrightText: © 2011, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
varying vec2 coord;


void main()
{

    coord = (gl_TextureMatrix[0] * gl_MultiTexCoord0).st;
    gl_Position = ftransform();

}
