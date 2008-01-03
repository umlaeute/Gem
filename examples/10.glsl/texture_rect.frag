// Cyrille Henry 2007

#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect MyTex;
//uniform sampler2D MyTex;

void main (void)
{
vec4 color = texture2DRect(MyTex, gl_TexCoord[0].st);
// vec4 color = texture2D(MyTex, gl_TexCoord[0].st);
 color = color* 0.5;
 gl_FragColor = color;
}
