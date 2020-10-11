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
//
#define PI 3.14159265
#define CONST1 sqrt(2.0)/2.0
//
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform bool reflective;
//
varying vec4 normalVec, lightVec, eyeVec;
//
vec4 PhongLighting()
{
	// Lookup the Light parameters
	vec4 Ia = gl_LightSource[0].ambient + gl_LightModel.ambient;
	vec4 Id = gl_LightSource[0].diffuse;
	vec4 Is = gl_LightSource[0].specular;
	//
	// Lookup the material properties in effect for this pixel
	vec4 Ie = gl_FrontMaterial.emission;
	vec4 Ka = gl_FrontMaterial.ambient;
	vec4 Kd = gl_FrontMaterial.diffuse;
	vec4 Ks = gl_FrontMaterial.specular;
	float n = gl_FrontMaterial.shininess;
	//
	// Normalize Phong vector parameters
	vec4 N = normalize(normalVec);
	vec4 L = normalize(lightVec);
	vec4 E = normalize(eyeVec);
	//
	vec4 Amb = Ka * Ia;
	float maxL = max(0.0, dot(N, L));
	//
	vec4 R;
	vec4 Diff;
	//
	// If the given object is the reflective object then calculate the direction of reflection and use it to access the reflection map
	if(reflective)
	{
		// Calculate direction of reflection
		R = 2.0 * dot(N, E) * N - E;
		float len = length(R.xyz);
		vec4 pos = R / len;
		//
		float a, b;
		// Test for which reflection component to use
		if(((pos.z >= 0.0) && (abs(pos.y) <= CONST1)) || ((pos.z <= 0.0) && (abs(pos.x) >= CONST1)))
		{
			// Calculte the position value in the map to use for lookup
			a = asin(pos.y)/(PI / 4.0);
			b = atan(pos.x, pos.z)/(3.0 * PI / 4.0);
			//
			// Scale the position value to the dimenstion of the reflection map
			vec2 uv = vec2(a, b);
			uv *= 0.5;
			uv += 0.5;
			uv = clamp(uv, 0.001, 0.999);
			//
			// Use the value to look up the color value from the reflection map
			Diff = texture2D(texture0, uv);
		}
		else
		{
			// Calculte the position value in the map to use for lookup
			a = asin(pos.x)/(PI / 4.0);
			b = atan(pos.y, -pos.z)/(3.0 * PI / 4.0);
			//
			// Scale the position value to the dimenstion of the reflection map
			vec2 uv = vec2(a, b);
			uv *= 0.5;
			uv += 0.5;
			uv = clamp(uv, 0.001, 0.999);
			//
			// Use the value to look up the color value from the reflection map
			Diff = texture2D(texture1, uv);
		}
		//
		// Calculate the phong lighting using the looked up color value and return the value
		float maxE = max(0.0, dot(R, E));
		vec4 Spec = Ks * Is * pow(maxE, n);
		//
		if(maxL == 0.0)
		{
			Spec = vec4(0.0, 0.0, 0.0, 0.0);
		}
		return(Ie + Amb * 0.4 + Diff + Spec);
	}
	else
	{
		// For non-reflective objects, do the normal phong lighting calculation and return the value
		R = ((2.0 * N) * maxL) - L;
		Diff = Kd * Id * maxL;
		//
		float maxE = max(0.0, dot(R, E));
		vec4 Spec = Ks * Is * pow(maxE, n);
		//
		if(maxL == 0.0)
		{
			Spec = vec4(0.0, 0.0, 0.0, 0.0);
		}
		return(Ie + Amb + Diff + Spec);
	}
}

void main()
{
	gl_FragColor = PhongLighting();
}
