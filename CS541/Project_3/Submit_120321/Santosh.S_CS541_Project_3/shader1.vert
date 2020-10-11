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

varying vec4 normalVec, lightVec, eyeVec;

#define PI 3.14159265

void main()
{
	//Calculate Phong lighting parameters
	vec4 R = gl_ModelViewMatrix * gl_Vertex;
	normalVec = vec4(normalize(gl_NormalMatrix * gl_Normal), 0.0);
	lightVec = normalize(gl_LightSource[0].position - R);
	eyeVec = normalize(-R);
	//
	float len = length(R.xyz);
	vec4 pos = R / len;
	//
	//Unwrap the given position value into a straight projection value
	float a, b, c;
	if(component == 0)
	{
		a = asin(pos.y)/(PI / 4.0);
		b = atan(pos.x, pos.z)/(3.0 * PI / 4.0);
		c = (len/100.0) + (b*b) - 0.75;
	}
	else if(component == 1)
	{
		a = asin(pos.x)/(PI / 4.0);
		b = atan(pos.y, -pos.z)/(3.0 * PI / 4.0);
		c = (len/100.0) + (b*b) - 0.75;
	}
	//
	gl_Position = vec4(a, b, c, 1.0);
}
