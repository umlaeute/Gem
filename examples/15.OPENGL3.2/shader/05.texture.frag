#version 460
;
// Cyrille Henry 2024;
// based on "OpenGL 4 Shading Language Cookbook" by David Wolff;
;
in vec2 TexCoord;
;
uniform sampler2D Texture1; // Texture 1 with using the textunit 0
uniform sampler2D Texture2; // Texture 2 need to be set to 1 to use the correct texture;
uniform float mix_factor;
;
out vec4 FragColor;
;
void main() {
	vec4 color1 = texture(Texture1, TexCoord);
	vec4 color2 = texture(Texture2, TexCoord);
;
	FragColor = mix(color1, color2, mix_factor);
}
