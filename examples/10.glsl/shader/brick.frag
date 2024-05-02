varying vec3 BrickColor_f, MortarColor_f;
varying vec2 BrickSize_f, BrickPct_f;

varying vec2 MCposition;
varying float LightIntensity;


void main()
{
   vec3 color;
   vec2 position, useBrick;
   position = MCposition / BrickSize_f;

   if (fract(position.y * 0.5) > 0.5)
      position.x += 0.5;

   position = fract(position);

   useBrick = step(position, BrickPct_f);

   color    = mix(MortarColor_f, BrickColor_f, useBrick.x * useBrick.y);
   color   *= LightIntensity;
   gl_FragColor = vec4(color, 1.0);
}