// Cyrille Henry 2007

#extension GL_ARB_texture_rectangle : enable

uniform float K; 
uniform sampler2DRect texture, texture1, texture2;

const float dx = 1.; // change to put the size of the openGL window
const float dy = 1.; // change to put the size of the openGL window
const float dp = 1.; 

void main (void)
{
	float light;
	vec2 position = gl_TexCoord[0].st;
	vec4 C, C1;
	C  = texture2DRect(texture2, vec2(1.33,1.)*position) -vec4(0.5); ;

	vec4 color2 = texture2DRect(texture1, vec2(1.33,1.)*(position+K*C.rg));

	gl_FragColor = color2;

}
