// Cyrille Henry 2008
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect texture_mass;
uniform float init;

void main (void)
{
	vec2 coord = 128.*gl_TexCoord[0].st;
	vec4 pos = texture2DRect(texture_mass, coord);

	gl_FragColor = pos;
}
