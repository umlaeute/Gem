// SPDX-FileCopyrightText: © 2007, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later

#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect MyTex;
//uniform sampler2D MyTex;
uniform float B,C;

void main (void)
{
 vec4 color = texture2DRect(MyTex,  (gl_TextureMatrix[0] * gl_TexCoord[0]).st);

 color *= B+1.; // brightness
 vec4 gray = vec4(dot(color.rgb,vec3(0.2125,  0.7154, 0.0721)));
 color = mix(gray, color, C+1.); // contrast
 gl_FragColor = color;
}
