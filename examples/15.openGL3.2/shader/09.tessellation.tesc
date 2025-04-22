#version 400
// Cyrille Henry 2025

layout(vertices = 3) out; // traw triangle

in VS_OUT {
    vec3 pos;
    vec3 norm;
    vec3 color;
} tcs_in[];

out TCS_OUT {
    vec3 pos;
    vec3 norm;
    vec3 color;
} tcs_out[];

void main() {
    // tessellarisation level
    gl_TessLevelInner[0] = 5.0;
    gl_TessLevelOuter[0] = 5.0;
    gl_TessLevelOuter[1] = 5.0;
    gl_TessLevelOuter[2] = 5.0;

    // Pass this data to control vertex
    tcs_out[gl_InvocationID].pos = tcs_in[gl_InvocationID].pos;
    tcs_out[gl_InvocationID].norm = tcs_in[gl_InvocationID].norm;
    tcs_out[gl_InvocationID].color = tcs_in[gl_InvocationID].color;
}

