///////////////////////////////////////////////////////////////////////
// Pass 2 fragment shader
//
// Gary Herron
//
// Copyright © 2011 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

// This is invoked to produce the shading for both normal objects and
// reflective objects.  Normal objects require only Phong lighting
// calculations.  Reflective objects use the normal to calculate the
// reflective direction R and use that to index the (one or more)
// reflective maps to produce a output color.  The reflective object
// could calculate its own (Phong) color and blend it with the reflective
// map data if desired.

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform bool reflective;

varying vec3 normalVec;

void main()
{
	vec3 N = normalize(normalVec);
	//vec3 E = normalize(eyeVec);
	//vec3 R = 2.0*dot(N,E)*N-E;

	gl_FragColor = N[2]*gl_FrontMaterial.diffuse;

}
