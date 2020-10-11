///////////////////////////////////////////////////////////////////////
// Pass 2 vertex shader
//
// Gary Herron
//
// Copyright © 2012 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

varying vec4 normalVec, lightVec, eyeVec, shadowCoord;

void main()
{
	//Calculate the Shadow Coordinate of each vertex
	vec4 R = gl_ModelViewMatrix * gl_Vertex;
	// Read the Shadow Transformation from the texture matrix and use it to calculate the shadow coordinate
	shadowCoord = gl_TextureMatrix[0] * R;
	gl_Position = ftransform();
	//
	// Calculate normalVec, lightVec, eyeVec required for Phong lighting
	normalVec = vec4(normalize(gl_NormalMatrix * gl_Normal), 0.0);
	lightVec = normalize(gl_LightSource[0].position - R);
	eyeVec = normalize(-R);
}
