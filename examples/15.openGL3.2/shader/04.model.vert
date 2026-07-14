// SPDX-FileCopyrightText: © 2024, Cyrille Henry and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
#version 460

layout (location=0) in vec3 position;
layout (location=1) in float colorR;
layout (location=2) in float colorG;
layout (location=3) in float colorB;

uniform mat4 transformation_matrix;

out vec4 Color;
out vec3 Normal;
out vec4 Position;

void main()
{
	Color = vec4(colorR, colorG, colorB, 1.);
	vec3 pos = vec3(position.x, position.y, position.z);
	Position = transformation_matrix * vec4(pos,1.0);
	gl_Position = Position;
}
