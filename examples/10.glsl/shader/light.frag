// Cyrille Henry 2024
// based on "OpenGL 4 Shading Language Cookbook" by David Wolff

// from vertex shader
varying vec3 Normal;
varying vec3 Position;

// light description
uniform vec4 LightPosition;  // Light position in eye coords.
uniform vec3 LightLa;        // Ambient light intensity
uniform vec3 LightL;         // Diffuse and specular light intensity

// material definition
uniform vec3 MaterialKa;            // Ambient reflectivity
uniform vec3 MaterialKd;            // Diffuse reflectivity
uniform vec3 MaterialKs;            // Specular reflectivity
uniform float MaterialShininess;    // Specular shininess factor

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
    spec = MaterialKs * pow( max( dot(h,n), 0.0 ), MaterialShininess );
  }
  return ambient + LightL * (diffuse + spec);
}

void main (void)
{
    gl_FragColor = vec4(blinnPhong(Position, normalize(Normal)), 1.0);
    //gl_FragColor = vec4(blinnPhong(Position, Normal), 1.0);
}
