#version 400
// Cyrille Henry 2025

layout(location = 0) in vec3 position;

out VS_OUT {
    vec3 pos;
    vec3 norm;
    vec3 color;
} vs_out;

void main() {
    vs_out.pos = position;
    vs_out.norm = vec3(0.,0.,1.);
    gl_Position = vec4(position, 1.0);
}
