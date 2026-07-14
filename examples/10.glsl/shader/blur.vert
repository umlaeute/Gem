// SPDX-FileCopyrightText: © 2011, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
varying vec2 texcoord0;
void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;

    // perform standard transform on vertex
    gl_Position = ftransform();

}
