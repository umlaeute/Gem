// SPDX-FileCopyrightText: © 2009, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
void main()
{
	vec4 tmp = gl_Color;
	tmp.a /= 10.;
	gl_FragColor = tmp;
	// set color but alpha is 20 time less
}
