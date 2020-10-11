///////////////////////////////////////////////////////////////////////
// Pass 1 fragment shader
//
// Gary Herron
//
// Copyright © 2012 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

// This is invoked to produce a Phong shaded image of objects from the
// viewpoint of the center of reflection. The result goes into a
// component of the reflection map.

uniform bool reflective;


void main()
{
	gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);

}
