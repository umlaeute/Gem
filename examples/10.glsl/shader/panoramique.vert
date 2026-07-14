// SPDX-FileCopyrightText: © 2011, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
void main()
{
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
