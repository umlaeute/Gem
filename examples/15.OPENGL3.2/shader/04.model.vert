#version 460

// Cyrille Henry 2024


layout (location=0) in float positionX;
layout (location=1) in float positionY;
layout (location=2) in float positionZ;
layout (location=3) in float normal_X;
layout (location=4) in float normal_Y;
layout (location=5) in float normal_Z;
layout (location=6) in float colorR;
layout (location=7) in float colorG;
layout (location=8) in float colorB;

uniform mat4 transformation_matrix;

out vec4 Color;
out vec3 Normal;
out vec4 Position;

void main()
{
	Color = vec4(colorR, colorG, colorB, 1.);
	Normal = vec3(normal_X, normal_Y, normal_Z);
	vec3 pos = vec3(positionX, positionY, positionZ);
	Position = transformation_matrix * vec4(pos,1.0);
	gl_Position = Position;
}
