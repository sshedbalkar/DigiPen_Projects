///////////////////////////////////////////////////////////////////////
// Pass 2 vertex shader
//
// Gary Herron
//
// Copyright © 2012 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

// This is invoked to produce imagesd of both normal objects and
// reflective objects.  It is expected to do the normal ftransform as
// well as all the Phong vertex processing.

varying vec4 normalVec, lightVec, eyeVec;

void main()
{
	//Calculate Phong lighting parameters
	vec4 R = gl_ModelViewMatrix * gl_Vertex;
	gl_Position = ftransform();
	//
	// Calculate normalVec, lightVec, eyeVec here
	normalVec = vec4(normalize(gl_NormalMatrix * gl_Normal), 0.0);
	lightVec = normalize(gl_LightSource[0].position - R);
	eyeVec = normalize(-R);
}
