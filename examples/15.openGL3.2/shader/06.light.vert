#version 460

// Cyrille Henry 2025

layout (location=0) in float positionX;
layout (location=1) in float positionY;
layout (location=2) in float positionZ;
layout (location=3) in float normal_X;
layout (location=4) in float normal_Y;
layout (location=5) in float normal_Z;
layout (location=6) in float colorR;
layout (location=7) in float colorG;
layout (location=8) in float colorB;

uniform mat4 modelMatrix;      // Transforms from object space to world space
uniform mat4 viewMatrix;       // Transforms from world space to camera space
uniform mat4 projMatrix;       // Transforms from camera space to clip space

out vec4 Color;
out vec3 Normal;
out vec3 FragPos;        // Position in world space for lighting calculations
out vec3 ViewPos;        // Position in view space

void main()
{
    // Pass color as is
    Color = vec4(colorR, colorG, colorB, 1.0);
    
    // Position in object space
    vec4 objectPos = vec4(positionX, positionY, positionZ, 1.0);
    
    // Position in world space - for lighting calculations
    vec4 worldPos = modelMatrix * objectPos;
    FragPos = worldPos.xyz;
    
    // Position in view space
    vec4 viewPos = viewMatrix * worldPos;
    ViewPos = viewPos.xyz;
    
    // Final position in clip space - what the rendering hardware uses
    gl_Position = projMatrix * viewPos;
    // Equivalent to: gl_Position = projMatrix * viewMatrix * modelMatrix * objectPos;
    
    // Normal transformation - uses only the model matrix for lighting calculations in world space
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    Normal = normalize(normalMatrix * vec3(normal_X, normal_Y, normal_Z));
}
