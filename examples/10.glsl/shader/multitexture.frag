// SPDX-FileCopyrightText: © 2007, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
uniform sampler2D MyTex0;
uniform sampler2D MyTex1;

varying vec2 texcoord0;
varying vec2 texcoord1;

void main (void)
{
 vec4 color0 = texture2D(MyTex0, texcoord0);
 vec4 color1 = texture2D(MyTex1, texcoord1);
 gl_FragColor = (color0 + color1) / 2.;
}
