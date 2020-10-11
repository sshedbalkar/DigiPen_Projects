///////////////////////////////////////////////////////////////////////
// Pass 1 fragment shader
//
// Gary Herron
//
// Copyright © 2012 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

varying vec4 position;

void main()
{
	// write the interpolated depth value of light penetration into the shadow map
	gl_FragColor = vec4(0.0, 0.0, 0.0, position.w);
	//gl_FragColor = vec4((100.0 * (position.z/position.w-0.99)).xxx, 1.0);
}
