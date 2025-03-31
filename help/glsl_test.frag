// Cyrille Henry 2024
// based on "OpenGL 4 Shading Language Cookbook" by David Wolff

// from vertex shader
varying vec3 Normal;
varying vec3 Position;

// light description
const vec4 LightPosition = vec4(5.,3.,1.,1.);  // Light position in eye coords.
const vec3 LightLa = vec3(1.,1.,1.);        // Ambient light intensity
const vec3 LightL = vec3(1.,1.,1.);         // Diffuse and specular light intensity

// material definition
const vec3 MaterialKa = vec3(.1, .1, .5);            // Ambient reflectivity
const vec3 MaterialKd = vec3(.8, .1, .1);            // Diffuse reflectivity
const vec3 MaterialKs = vec3(.1, .3, .1);            // Specular reflectivity
uniform float shininess;    // Specular shininess factor

// The lighting model
vec3 blinnPhong( vec3 position, vec3 n) {
  vec3 ambient = LightLa * MaterialKa;
  vec3 s = normalize( LightPosition.xyz - position );
  float sDotN = max( dot(s,n), 0.0 );
  vec3 diffuse = MaterialKd * sDotN;
  vec3 spec = vec3(0.0);
  if( sDotN > 0.0 ) {
    vec3 v = normalize(-position.xyz);
    vec3 h = normalize( v + s );
    spec = MaterialKs * pow( max( dot(h,n), 0.0 ), shininess );
  }
  return ambient + LightL * (diffuse + spec);
}

void main (void)
{
    gl_FragColor = vec4(blinnPhong(Position, normalize(Normal)), 1.0);
    //gl_FragColor = vec4(blinnPhong(Position, Normal), 1.0);
}
