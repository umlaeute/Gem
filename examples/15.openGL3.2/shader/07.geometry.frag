#version 150
// Cyrille Henry 2025

// from geometry shader
in vec3 geomPosition;
in vec3 geomNormal;
in vec2 geomTexCoord;
in vec3 barycentric;

// to rendering
out vec4 fragColor;

// uniform to compute the pixel color
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform sampler2D diffuseTexture;

void main() {
    // Normalize the normal vector
    vec3 normal = normalize(geomNormal);

    // Calculate the light direction and distance
    vec3 lightDir = normalize(lightPos - geomPosition);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Texture sampling
    vec4 texColor = texture(diffuseTexture, geomTexCoord);

    // Wire effect using barycentric coordinates
    float minBary = min(min(barycentric.x, barycentric.y), barycentric.z);
    float edgeFactor = smoothstep(0.0, 0.2, minBary);

    // Final color calculation
    vec3 result = (ambientColor + diffuse) * texColor.rgb;
    result = mix(vec3(0.0, 0.7, 1.0), result, edgeFactor); // Blue wireframe

    fragColor = vec4(result, texColor.a);
}
