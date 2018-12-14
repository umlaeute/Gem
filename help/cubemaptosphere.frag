// Antoine Rousseau september 2015
#version 120

uniform samplerCube envMap;
uniform float alpha = 1.0;

void main() {
  
    vec2 tc = gl_TexCoord[0].st ;
    vec2 thetaphi = ((tc * 2.0) - vec2(1.0)) * vec2(3.1415926535897932384626433832795, 1.5707963267948966192313216916398);
    float ctpy = cos(thetaphi.y);
    
    vec3 rayDirection = vec3(ctpy * cos(thetaphi.x), sin(thetaphi.y), ctpy * sin(thetaphi.x));
    
    gl_FragColor = textureCube(envMap, rayDirection);
    gl_FragColor.a *= alpha;
}
