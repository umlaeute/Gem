#version 460
// Cyrille Henry 2025

// Light in world space
uniform vec4 LightPosition;    // Light position in world space
uniform vec3 LightLa;          // Ambient intensity
uniform vec3 LightL;           // Diffuse/specular intensity
uniform vec3 CameraPosition;   // Camera position in world space

// Material
uniform vec3 MaterialKa;       // Ambient reflectivity
uniform vec3 MaterialKd;       // Diffuse reflectivity
uniform vec3 MaterialKs;       // Specular reflectivity
uniform float MaterialShininess; // Shininess

in vec4 Color;
in vec3 Normal;
in vec3 FragPos;    // Fragment position in world space
in vec3 ViewPos;    // Position in view space (for debugging)

out vec4 FragColor; // Final color

vec3 blinnPhong(vec3 position, vec3 n) {

    // Ambient component
    vec3 ambient = LightLa * MaterialKa;

    // Vector to the light
    vec3 lightDir = normalize(LightPosition.xyz - position);

    // Diffuse component
    float diff = max(dot(n, lightDir), 0.0);
    vec3 diffuse = MaterialKd * diff;

    // Vector to the viewer (camera)
    vec3 viewDir = normalize(CameraPosition - position);

    // Specular component
    vec3 specular = vec3(0.0);
    if (diff > 0.0) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(n, halfwayDir), 0.0), MaterialShininess);
        specular = MaterialKs * spec;
    }

    return ambient + LightL * (diffuse + specular);
}

void main() {
    vec3 lighting = blinnPhong(FragPos, normalize(Normal));
    FragColor = Color * vec4(lighting, 1.0);
}

