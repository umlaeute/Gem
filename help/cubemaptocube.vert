// cubemap direction mapping
#version 120

varying vec3 texcoords;

void main() {
  texcoords = vec3(gl_Vertex);
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
