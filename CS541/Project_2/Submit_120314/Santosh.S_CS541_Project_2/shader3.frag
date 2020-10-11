uniform int mode;           // Set by the application to an integer 0-9.

varying vec3 normalVec, lightVec, eyeVec;// Remember to normalize before using!
varying vec3 Pu;
//
uniform sampler2D tileColor;
uniform sampler2D tileHeight;
//
vec4 PhongLightingWithBumpMap()
{
	// Lookup the Light parameters
	vec4 Ia = gl_LightSource[0].ambient + gl_LightModel.ambient;
	vec4 Id = gl_LightSource[0].diffuse;
	vec4 Is = gl_LightSource[0].specular;
	//
	// Lookup the material properties in effect for this pixel
	vec4 Ie = gl_FrontMaterial.emission;
	vec4 Ks = gl_FrontMaterial.specular;
	float n = gl_FrontMaterial.shininess;
	//
	// Normalize vectors
	vec3 N = normalize(normalVec);
	vec3 L = normalize(lightVec);
	vec3 E = normalize(eyeVec);
	//
	// Calculate the bi-normal
	vec3 PuN;
	PuN = normalize(Pu);
	vec3 Pv = cross(N, PuN);
	//
	// Divide the sphere into 16 longitudes and 16 latitudes
	vec2 tVec2 = vec2(gl_TexCoord[0].s * 16.0f, gl_TexCoord[0].t * 16.0f);
	//
	// Lookup the texture color of the pixel
	vec4 Ka = texture2D(tileColor, tVec2);
	vec4 Kd = texture2D(tileColor, tVec2);
	//
	// Calculate the height of a region around the pixel
	float d = 1.0f / 256.0f;
	float hv = texture2D(tileHeight, vec2(tVec2.s + d, tVec2.t)).x;
	float hu = texture2D(tileHeight, vec2(tVec2.s, tVec2.t + d)).x;
	//
	// Lookup the height value of the pixel
	float h = texture2D(tileHeight, tVec2).x;
	//
	// Calculate the height derivative around the pixel
	float fv = (hv - h) / d;
	float fu = (hu - h) / d;
	//
	// Calculate the perturbed normal and normalize it
	float s = 0.2f;
	N = N + (s * cross((fv * PuN - fu * Pv), N));
	vec3 N1 = normalize(N);
	//
	// Do the Phong light calculation with the new perturbed normal
	vec4 Amb = Ka * Ia;
	float maxL = max(0.0f, dot(N1, L));
	//
	vec4 Diff = Kd * Id * maxL;
	//
	vec3 R = ((2.0f * N1) * maxL) - L;
	float maxE = max(0.0f, dot(R, E));
	vec4 Spec = Ks * Is * pow(maxE, n);
	//
	if(maxL == 0.0f)
	{
		Spec = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	return(Ie + Amb + Diff + Spec);
}
//
void main()
{
	gl_FragColor = PhongLightingWithBumpMap();
}
