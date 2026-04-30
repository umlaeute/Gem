varying vec2 texcoord0;
varying vec2 texcoord1;

void main()
{

    texcoord0 = (gl_TextureMatrix[0] * gl_MultiTexCoord0).st;
    texcoord1 = (gl_TextureMatrix[1] * gl_MultiTexCoord1).st;
    gl_Position = ftransform();

}
