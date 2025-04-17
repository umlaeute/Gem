#version 460

//simple test shader
// Cyrille Henry 2024
// based on "OpenGL 4 Shading Language Cookbook" by David Wolff

// This is the vertex shader, the 1st on the pipeline. It is executed for every vertex.

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;
// declare the mandatory input : we need 2 attributes named position and color from the VBO
// location must match the program_index from gemvertexbuffer (thanks to the print_attribute message)

out vec3 Color_to_frag;
// the output of this shader is only a color.
// This variable will be interpolated between all vertices

void main()
{
	Color_to_frag = color;
	// initialise the variable to pass to the frag shader with data comming from the VBO

	gl_Position = vec4(position,1.0); // update vertex position from the VBO
	// No perspective is apply in this example
}
