#version 400
// Cyrille Henry 2025

layout(triangles, equal_spacing, cw) in;

in TCS_OUT {
    vec3 pos;
    vec3 norm;
    vec3 color;
} tes_in[];

out VS_OUT {
    vec3 pos;
    vec3 norm;
    vec3 color;
} vs_out;

void main() {
    // interpolate vertex position
    vec3 p0 = tes_in[0].pos;
    vec3 p1 = tes_in[1].pos;
    vec3 p2 = tes_in[2].pos;

    vec3 n0 = tes_in[0].norm;
    vec3 n1 = tes_in[1].norm;
    vec3 n2 = tes_in[2].norm;

    vec3 pos = (gl_TessCoord.x * p0) + (gl_TessCoord.y * p1) + (gl_TessCoord.z * p2);
    vec3 norm = normalize((gl_TessCoord.x * n0) + (gl_TessCoord.y * n1) + (gl_TessCoord.z * n2));

    vs_out.pos = pos;
    vs_out.norm = norm;
    gl_Position = vec4(pos, 1.0);
    vec3 mycolor; // custom pseudo random color at each vertices after subdivision
    mycolor.rg = cos(1.+12.12*pos.xy);
    mycolor.b = cos(1.+12.12*pos.x*pos.y);    
    vs_out.color = vec3(0.5)+0.5*mycolor;
}

