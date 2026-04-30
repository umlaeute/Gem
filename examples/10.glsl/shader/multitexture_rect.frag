#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect MyTex0;
uniform sampler2DRect MyTex1;

varying vec2 texcoord0;
varying vec2 texcoord1;

void main (void)
{
 vec4 color0 = texture2DRect(MyTex0, texcoord0);
 vec4 color1 = texture2DRect(MyTex1, texcoord1); 
 gl_FragColor = (color0 + color1) / 2.;
}
