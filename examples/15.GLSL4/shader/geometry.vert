// Vertex Shader (version 150)
#version 150

// from VBO
in vec3 position;
in vec3 normal;
in vec2 texCoord;

// to geometry shader
out vec3 vertPosition;
out vec3 vertNormal;
out vec2 vertTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;

void main() {
    vertPosition = vec3(modelMatrix * vec4(position, 1.0));
    vertNormal = normalMatrix * normal;
    vertTexCoord = texCoord;
    
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0); 
}
