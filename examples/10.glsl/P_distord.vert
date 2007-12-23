// Cyrille Henry 2007

uniform float K;
uniform sampler2D tex0;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;

	float xs1 = sin(gl_TexCoord[0].s*gl_TexCoord[0].t);
	float xs2 = sin(xs1*533.);
	float xs3 = K * sin(xs2*1013.);

	float ys1 = sin(gl_TexCoord[0].s*gl_TexCoord[0].t);
	float ys2 = sin(ys1*5313.);
	float ys3 = K * sin(ys2*10113.);
		
	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + vec4(xs3,ys3,0.,0.));
	
}

