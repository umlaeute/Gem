// SPDX-FileCopyrightText: © 2009, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
void main()
{
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
	//simple passthrow
}
