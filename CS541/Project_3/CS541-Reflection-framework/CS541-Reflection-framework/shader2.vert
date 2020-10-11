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

varying vec3 normalVec;

void main()
{       
	normalVec = gl_NormalMatrix*gl_Normal;
	gl_Position = ftransform();
}
