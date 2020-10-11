uniform int mode;           // Set by the application to an integer 0-9.

varying vec3 normalVec, lightVec, eyeVec, Pu;
uniform sampler2D tile_color; 
uniform sampler2D tile_height;


void main()
{
	//normalizing the three vectors
	vec3 L = normalize(lightVec);   
	vec3 E = normalize(eyeVec);
	vec3 N = normalize(normalVec);
	
	// Lookup the Light parameters
    vec4 Ia = gl_LightSource[0].ambient + gl_LightModel.ambient;
    vec4 Id = gl_LightSource[0].diffuse;
    vec4 Is = gl_LightSource[0].specular;
    
    // Lookup the material properties in effect for this pixel
    vec4 Ie = gl_FrontMaterial.emission;
    vec4 Ks = gl_FrontMaterial.specular;
    float n = gl_FrontMaterial.shininess;
	
	vec3 pv=cross(N,Pu);
	pv=normalize(pv);
	vec3 pu=normalize(Pu);

	vec2 h = vec2( float (int(gl_TexCoord[0].s * 1024) % 64) / 64.0,float (int(gl_TexCoord[0].t * 512) % 64) / 64.0);
	
	vec4 Ka = texture2D(tile_color, h);
	vec4 Kd = texture2D(tile_color, h);

	float d = 256.0f;
	float i = floor(h.x*d);
	float j = floor(h.y*d);
	float u_0 = 1 + i - (h.x*d);
	float v_0 = 1 + j - (h.y*d);
	float u_1 = (h.x * d) - i;
	float v_1 = (h.y * d) - j;
	float hu_1=(d * v_0) * (texture2D(tile_height, vec2((i + 1)/d, j/d)).r - texture2D(tile_height, vec2(i/d, j/d)).r);
	float hu_2=(d * v_1) * (texture2D(tile_height, vec2((i + 1)/d, (j+1)/d)).r - texture2D(tile_height, vec2(i/d, (d+1)/d)).r);
	float hv_1=(d * u_0) * (texture2D(tile_height, vec2(i/d, (j+1)/d)).r - texture2D(tile_height, vec2(i/d, j/d)).r);
	float hv_2=(d * u_1) * (texture2D(tile_height, vec2((i + 1)/d, (j+1)/d)).r - texture2D(tile_height, vec2((i+1)/d, j/d)).r);
	float fu = hu_1 + hu_2;
	float fv = hv_1 + hv_2;
	
	N += (0.1 * cross((fv * pu) - (fu * pv), N));
	N = normalize(N);
	

	vec4 A=Ka*Ia;
	float nDotL=max(0.0,dot(N,L));
	vec4 D=Kd*Id*nDotL;
	vec4 S= Ks*Is*pow(max(0.0,dot(((2.0*N)*nDotL-L),E)),n);
	if(nDotL==0.0) S=vec4(0.0, 0.0, 0.0, 0.0);

	gl_FragColor = Ie+A+D+S;
}
