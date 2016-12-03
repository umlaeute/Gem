// cubemap direction mapping
#version 120

varying vec3 texcoords;
uniform samplerCube cube_texture;

void main() {
	gl_FragColor = textureCube(cube_texture, texcoords);
}
