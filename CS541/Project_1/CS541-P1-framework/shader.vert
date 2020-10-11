// gl_Normal: Model space normal
// gl_NormalMatrix: Transforms normals Model-space ==> Eye-space

// gl_Vertex: Model space vertex 
// gl_ModelViewMatrix: Transforms vertex Model-space ==> Eye-space
// gl_ProjectionMatrix: Transforms vertex Eye-space ==> Projection-space
// ftransform(): Equiv to gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex

// gl_LightSource[0].position: Light position, transformed into Eye-space

varying vec3 normalVec, lightVec, eyeVec;

void main()
{
  gl_Position = ftransform();
  // Calculate normalVec, lightVec, eyeVec here
}
