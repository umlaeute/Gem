#version 460

//simple test shader
// Cyrille Henry 2024
// based on "OpenGL 4 Shading Language Cookbook" by David Wolff

// This is the vertex shader, the 1st on the pipeline. It is executed for every vertex.

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;
// declare the mandatory input : we need 2 attributes named position and color from the VBO
// location must match the program_index from gemvertexbuffer (thanks to the print_attribute message)

uniform mat4 transformation_matrix;
// The transformation matrix we want to apply to the vertex.
// The shader did not receive the transformation matrix from the pipeline, so we need to creater one and pass it as an argument.
// In this example, we use gemlist_matrix in the patch to get the transformation matrix.
// The perspective matrix (set with the perspec message to gemwin) is not used.


out vec3 Color_to_frag;
// the output of this shader is only a color.
// This variable will be interpolated between all vertices

void main()
{
	Color_to_frag = color;
	// initialise the variable to pass to the frag shader with data comming from the VBO

	gl_Position = transformation_matrix * vec4(position,1.0); // update vertex position from the VBO
	// No perspective is apply in this example
}
