// Cyrille Henry 2008
uniform sampler2D texture;
// varying vec4 C;
uniform float K2;


vec2 pos_(vec2 V, float x, float y)
{
	V += vec2(x/128.,y/128.);
	return(max(min(V,vec2(91./128.,63./128.)),vec2(0.)));
}


void main (void)
{
	vec2 pos = gl_TexCoord[0].st * vec2(90./128.,62./128.) ; 

	vec2 posBG = (floor(pos*vec2(128.,128.)))/128.; 

	vec4 color1 = texture2D(texture,pos_(posBG,-1.,-1.));
	vec4 color2 = texture2D(texture,pos_(posBG, 0.,-1.));
	vec4 color3 = texture2D(texture,pos_(posBG, 1.,-1.));
	vec4 color4 = texture2D(texture,pos_(posBG,-1., 0.));
	vec4 color5 = texture2D(texture,pos_(posBG, 0., 0.));
	vec4 color6 = texture2D(texture,pos_(posBG, 1., 0.));
	vec4 color7 = texture2D(texture,pos_(posBG,-1., 1.));
	vec4 color8 = texture2D(texture,pos_(posBG, 0., 1.));
	vec4 color9 = texture2D(texture,pos_(posBG, 1., 1.));

	vec4 colorBG = color1 + color2 + color4 + color5;
	vec4 colorBD = color3 + color2 + color6 + color5; 
	vec4 colorHG = color4 + color5 + color7 + color8; 
	vec4 colorHD = color5 + color6 + color8 + color9;

	pos = (pos-posBG) * 128.;

	vec4 XB = mix(colorBG,colorBD,pos.x);
	vec4 XH = mix(colorHG,colorHD,pos.x);
	vec4 X = mix(XB,XH,pos.y) / 4.;

	X -= vec4(0.5);
	X.xyz = normalize(X.xyz);

	if(gl_FrontFacing)
	{X *= -1.;}

	float tmp =  dot(X.xyz,normalize(vec3(0.,1.,1.)));
	tmp = -tmp;
	tmp = max(0.,tmp) + 0.3 * min(0.,tmp);

	tmp = 0.1 + 0.9 * tmp;
	vec4 color = tmp * vec4(1,0.95,0.9,1);

	color.a = smoothstep(0.035,0.04,gl_TexCoord[0].s);

	gl_FragColor = color;
}


