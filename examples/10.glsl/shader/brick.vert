attribute vec3 LightPosition;
attribute vec3 BrickColor, MortarColor;
attribute vec2 BrickSize, BrickPct;

varying   vec3 BrickColor_f, MortarColor_f;
varying   vec2 BrickSize_f, BrickPct_f;


const float SpecularContribution = 0.3;
const float DiffuseContribution = 1.0 - SpecularContribution;

varying float LightIntensity;
varying vec2  MCposition;

void main()
{
   // conversions
   BrickColor_f = BrickColor;
   MortarColor_f = MortarColor;
   BrickSize_f = BrickSize;
   BrickPct_f = BrickPct;
   //

   vec3 ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
   vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);
   vec3 lightVec   = normalize(LightPosition - ecPosition);
   vec3 reflectVec = reflect(-lightVec, tnorm);
   vec3 viewVec    = normalize(-ecPosition);
   float diffuse   = max(dot(lightVec, tnorm), 0.0);
   float spec      = 0.0;

   if (diffuse > 0.0)
   {
      spec = max(dot(reflectVec, viewVec), 0.0);
      spec = pow(spec, 16.0);
   }

   LightIntensity  = DiffuseContribution * diffuse +
   		     SpecularContribution * spec;

   MCposition      = gl_Vertex.xy;
   gl_Position     = ftransform();
}
