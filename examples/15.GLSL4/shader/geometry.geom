#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// from vertex shader
in vec3 vertPosition[];
in vec3 vertNormal[];
in vec2 vertTexCoord[];

// to fragment shader
out vec3 geomPosition;
out vec3 geomNormal;
out vec2 geomTexCoord;
out vec3 barycentric;

void main() {
    for(int i = 0; i < 3; i++) {
        geomPosition = vertPosition[i];
        geomNormal = vertNormal[i];
        geomTexCoord = vertTexCoord[i];
        
        // Passing barycentric coordinates for fragment shading
        barycentric = vec3(0.0);
        barycentric[i] = 1.0;
        
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
