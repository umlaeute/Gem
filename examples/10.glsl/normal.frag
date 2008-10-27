// Cyrille Henry 2008
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect texture_mass, texture_image;
uniform float init;

vec2 test1(vec2 V)
{
	return(max(min(V,vec2(92.,64.)),vec2(0.)));
}

void main (void)
{
	vec4 color = vec4(0.);
	vec2 coord = gl_TexCoord[0].st;

	if ( (coord.x < 91./128.) && (coord.y <  63./128.) )
	{
		vec4 posG = texture2DRect(texture_mass, test1(coord*vec2(128.)+vec2(-1., 0.))) -vec4(0.5);
		vec4 posD = texture2DRect(texture_mass, test1(coord*vec2(128.)+vec2( 1., 0.))) -vec4(0.5);
		vec4 posH = texture2DRect(texture_mass, test1(coord*vec2(128.)+vec2( 0.,-1.))) -vec4(0.5);
		vec4 posB = texture2DRect(texture_mass, test1(coord*vec2(128.)+vec2( 0., 1.))) -vec4(0.5);
	
		vec3 normal = cross((posG.xyz-posD.xyz),(posH.xyz-posB.xyz));
		normal = normalize(normal);

		color.xyz = normal.xyz;

		color += 1.;
		color *= 0.5;
	}
	color.a = 1.;
	gl_FragColor = color;

}
