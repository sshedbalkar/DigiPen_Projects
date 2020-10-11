varying vec3 normalVec, lightVec, eyeVec, Pu;
attribute vec3 vertexTangent;

void main()
{
  gl_Position = ftransform();
  vec3 V;
  V = (gl_ModelViewMatrix * gl_Vertex).xyz;
  normalVec = gl_NormalMatrix * gl_Normal;
  eyeVec = -V;
  lightVec = gl_LightSource[0].position.xyz - V;
  Pu = gl_NormalMatrix * vertexTangent;
  gl_TexCoord[0] = gl_MultiTexCoord0;
}