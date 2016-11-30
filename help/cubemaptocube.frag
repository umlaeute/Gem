// cubemap direction mapping
#version 120

varying in vec3 texcoords;
uniform samplerCube cube_texture;

void main() {
	gl_FragColor = textureCube(cube_texture, texcoords);
}
