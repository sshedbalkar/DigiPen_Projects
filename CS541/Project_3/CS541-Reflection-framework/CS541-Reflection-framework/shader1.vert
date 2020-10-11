///////////////////////////////////////////////////////////////////////
// Pass 1 vertex shader
//
// Gary Herron
//
// Copyright © 2012 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

// This is invoked once for each component in the reflection map.  It
// transformationa a vertex in such a way so as to "unwrap" space into a
// straight projection onto each component.

uniform int component;

void main()
{       
	vec4 R = gl_ModelViewMatrix*gl_Vertex;

	gl_Position = ftransform();	/* Replace this temporary calculation. */
}
