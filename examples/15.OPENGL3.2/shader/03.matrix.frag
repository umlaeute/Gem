#version 150

// from geometry shader
in vec3 vertNormal;
in vec3 vertColor;

// to rendering
out vec4 FragColor;

void main() {
    // Normalize the normal vector
    vec3 normal = normalize(vertNormal);

    // color calculation
	float color = abs(dot(vec3(0., 0., 1.),vertNormal));
	
    FragColor = vec4(color*vertColor, 1.);
}
