// funcLightingOpenGL.vert

// i: gl light number
// vsVertexPosition: position of vertex in view space
vec4 calcBillboardLightingOpenGL(int i, vec4 vsVertexPosition)
{
	vec4 lightPosition = gl_LightSource[i].position;

	float attenuation = 1.0;
	if ( lightPosition.w > 0.0 )
	{
		// omni light. calculate attenuation.
		float d = length((lightPosition - vsVertexPosition).xyz);
	
		attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +
			gl_LightSource[i].linearAttenuation * d +
			gl_LightSource[i].quadraticAttenuation * d * d);
	}

	return gl_LightSource[i].diffuse * attenuation;
}

vec4 calcVertexLightingOpenGL( int i, vec4 vsVertexPosition )
{
	vec4 lightPosition = gl_LightSource[i].position;

	float attenuation = 1.0;
	if ( lightPosition.w > 0.0 )
	{
		// omni light. calculate attenuation.
		float d = length((lightPosition - vsVertexPosition).xyz);
	
		attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +
			gl_LightSource[i].linearAttenuation * d +
			gl_LightSource[i].quadraticAttenuation * d * d);
	}

	return gl_LightSource[i].diffuse * attenuation;
}