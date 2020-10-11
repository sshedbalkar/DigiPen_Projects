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

varying vec4 normalVec, lightVec, eyeVec;

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
	vec4 N = normalize(normalVec);
	vec4 L = normalize(lightVec);
	vec4 E = normalize(eyeVec);
	//
	vec4 Amb = Ka * Ia;
	float maxL = max(0.0, dot(N, L));
	//
	vec4 Diff = Kd * Id * maxL;
	//
	vec4 R = ((2.0 * N) * maxL) - L;
	float maxE = max(0.0, dot(R, E));
	vec4 Spec = Ks * Is * pow(maxE, n);
	//
	if(maxL == 0.0)
	{
		Spec = vec4(0.0, 0.0, 0.0, 0.0);
	}
	return(Ie + Amb + Diff + Spec);
}
void main()
{
	gl_FragColor = PhongLighting();
}
