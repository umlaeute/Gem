// Cyrille Henry 2007

uniform sampler2D texture;

const float dx = 1./500.; // change to gemwin dim

void main (void)
{	
	float x = gl_TexCoord[0].s;
	float y = gl_TexCoord[0].t;
	
	vec4 c;
	c  = texture2D(texture, vec2(x-dx, y-dx));
	c += texture2D(texture, vec2(x, y-dx));
	c += texture2D(texture, vec2(x+dx, y-dx));
	c += texture2D(texture, vec2(x-dx, y));
	c += texture2D(texture, vec2(x+dx, y));
	c += texture2D(texture, vec2(x-dx, y+dx));
	c += texture2D(texture, vec2(x, y+dx));
	c += texture2D(texture, vec2(x+dx, y+dx));

	vec4 c1 = texture2D(texture, vec2(x, y));

	c.rgb = vec3(step(2.5-c1.r,c.r)*step(c.r,3.5));
	c.a = 1.;

	gl_FragColor = c;
}
