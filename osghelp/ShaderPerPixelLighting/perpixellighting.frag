varying vec3 N;
varying vec3 v;

void main (void)
{
	// because the normal is varying (interpolated) we must now normalize
	// it in the fragment shader
	N = normalize(N);

	// this works only for omni lights!!! if you use directional lights
	// then you don't need to subtract v
	// calc vector from light to vertex (all in camera-space)
	vec3 L = normalize(gl_LightSource[0].position.xyz - v); 
	
	//calculate Diffuse Term:
	vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);

	//calculate Ambient Term:
	vec4 Iamb = gl_FrontLightProduct[0].ambient;

	gl_FragColor = Iamb + Idiff; 
}