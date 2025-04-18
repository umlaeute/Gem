#version 460

//simple test shader
// Cyrille Henry 2024
// based on "OpenGL 4 Shading Language Cookbook" by David Wolff

in vec3 position;
in vec2 texcoord;

out vec2 TexCoord;

void main()
{
	TexCoord = texcoord; // pass the data from VBO to the frag shader
	gl_Position = vec4(position,1.0);
}
