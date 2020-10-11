uniform int mode;           // Set by the application to an integer 0-9.

varying vec3 normalVec, lightVec, eyeVec;// Remember to normalize before using!

vec4 NotPhongLighting()
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
    return Kd;
}

void main()
{
	gl_FragColor = NotPhongLighting();
}
