#version 150
// Cyrille Henry 2025

// this shader add vertices between 2 vertices of the original geometry.
// It use 4 vertices input, in order to do a cubix interpolation, for smooth rendering.

layout(lines_adjacency) in;
layout(line_strip, max_vertices = 256) out;

// from vertex shader
in vec3 vertPosition[];

// to fragment shader
out vec3 geomPosition;

const int NUM_SEGMENTS = 8;

vec3 cubicInterpolate(vec3 p0, vec3 p1, vec3 p2, vec3 p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;
    
    vec3 a = -0.5 * p0 + 1.5 * p1 - 1.5 * p2 + 0.5 * p3;
    vec3 b = p0 - 2.5 * p1 + 2.0 * p2 - 0.5 * p3;
    vec3 c = -0.5 * p0 + 0.5 * p2;
    vec3 d = p1;
    
    return a * t3 + b * t2 + c * t + d;
}

void main() {
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;
    vec3 p3 = gl_in[3].gl_Position.xyz;
    
    //only generate a curve between point 1 and 2
    for (int j = 0; j <= NUM_SEGMENTS; j++) {
        float t = float(j) / float(NUM_SEGMENTS);
        vec3 interpolatedPosition = mix(p1, p2, t);
        // adjusting point position thanks to adjacency  points
        if (j > 0 && j < NUM_SEGMENTS) {
            vec3 cubicPos = cubicInterpolate(p0, p1, p2, p3, t);
            interpolatedPosition = cubicPos;
        }
        
        geomPosition = interpolatedPosition;
        gl_Position = vec4(interpolatedPosition, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}
