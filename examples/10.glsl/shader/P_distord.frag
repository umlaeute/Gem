// SPDX-FileCopyrightText: © 2007, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later

uniform sampler2D tex0;

void main()
{
    vec2 C =  (gl_TextureMatrix[0] * gl_TexCoord[0]).st;
    gl_FragColor = texture2D(tex0, C ) ;
}
