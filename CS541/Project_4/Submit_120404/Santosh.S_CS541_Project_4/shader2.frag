///////////////////////////////////////////////////////////////////////
// Pass 2 fragment shader
//
// Gary Herron
//
// Copyright © 2011 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////
//
uniform sampler2D shadowTexture;
//
varying vec4 normalVec, lightVec, eyeVec, shadowCoord;
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
	// calculate the coordinate value used to lookup the light's depth from the shadow map
	vec2 uv = vec2(shadowCoord.x / shadowCoord.w, shadowCoord.y / shadowCoord.w);
	// Bound the coordinate within 0.0 and 1.0
	uv = clamp(uv, 0.001, 0.999);
	// Use this value to lookup the ligt's depth into the scene from the shadow map
	vec4 shadow = texture2D(shadowTexture, uv);
	//
	// Light's depth and Pixel depth
	float depthPixel = shadowCoord.w;
	float depthLight = shadow.w;
	//
	// Compare Light's depth and Pixel depth (after adding a small quantity to account for floating point errors to remove the shadow acne)
	if(depthPixel > (depthLight + 0.0001))
	{
		// The pixel is in shadow, return the ambient light
		return(Amb);
	}
	else
	{
		// The pixel is not in shadow. Do the Phong lighting calculation on it
		vec4 R = ((2.0 * N) * maxL) - L;
		vec4 Diff = Kd * Id * maxL;
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
