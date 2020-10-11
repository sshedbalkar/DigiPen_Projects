uniform int mode;           // Set by the application to an integer 0-9.

varying vec3 normalVec, lightVec, eyeVec;// Remember to normalize before using!

vec4 PhongLighting()
{
    // Lookup the Light parameters
    vec4 Ia = gl_LightSource[0].ambient + gl_LightModel.ambient;
    vec4 Id = gl_LightSource[0].diffuse;
    vec4 Is = gl_LightSource[0].specular;
    
    // Lookup the material properties in effect for this pixel
    vec4 Ie = gl_FrontMaterial.emission;
    vec4 Ka = gl_FrontMaterial.ambient;
    vec4 Kd = gl_FrontMaterial.diffuse;
    vec4 Ks = gl_FrontMaterial.specular;
    float n = gl_FrontMaterial.shininess;
    
    // Return flat color (doing no calculation)
    //return Kd;
	vec3 N = normalize(normalVec);
	vec3 L = normalize(lightVec);
	vec3 E = normalize(eyeVec);
	//
	vec4 Amb = Ka * Ia;
	float maxL = max(0.0, dot(N, L));
	//
	vec4 Diff = Kd * Id * maxL;
	//
	vec3 R = ((2 * N) * maxL) - L;
	float maxE = max(0.0, dot(R, E));
	vec4 Spec = Ks * Is * pow(maxE, n);
	//
	if(maxL==0)
	{
		Spec = 0;
	}
	return(Ie + Amb + Diff + Spec);
}

void main()
{
	gl_FragColor = PhongLighting();
}
