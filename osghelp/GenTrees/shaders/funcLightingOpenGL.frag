// funcLightingOpenGL.frag


// OpenGL gl_LightSource[i].position.w
// w = 0 : directional
// w = 1 : point light

vec4 calcLightingDiffuse( vec3 n, vec4 v, const float w, const vec3 vPosLight, const vec4 vLightDiffuse, bool bUseFrontMaterialDiffuse, bool bUseBackMaterialDiffuse )
{
	// calc vector from vertex to light (all in camera-space)
	vec3 vv = vec3(0.0,0.0,0.0);
	if ( w>0.0 )
	{
		vv = v.xyz;
	}
	vec3 L = normalize( vPosLight-vv ); 
	
	// calculate diffuse terms
	vec4 ret = vec4(0,0,0,0);

	if ( !bUseFrontMaterialDiffuse && !bUseBackMaterialDiffuse )
	{
		return vLightDiffuse * max(dot(n,L), 0.0);
	}

	if ( bUseFrontMaterialDiffuse )
	{
		ret += vLightDiffuse * gl_FrontMaterial.diffuse * max(dot(n,L), 0.0);
	}

	if ( bUseBackMaterialDiffuse )
	{
		ret += vLightDiffuse * gl_BackMaterial.diffuse * max(dot(n,L), 0.0);
	}

	return ret;
}

vec3 calcLightingIntensityOpenGL( int i, vec4 v, vec3 n, bool bUseFrontMaterialDiffuse, bool bUseBackMaterialDiffuse )
{
	float w = 0.0;
	vec3 vPosLight;
	vec4 vLightDiffuse;
	vec4 Idiff;
	vec4 Iamb;
	
	w				= gl_LightSource[i].position.w;
	vPosLight		= gl_LightSource[i].position.xyz;
	vLightDiffuse	= gl_LightSource[i].diffuse;
	
	float att = 1.0;

	// omni light (point light), we need to calc the attenuation
	if ( w>0.0 )
	{
		vec3 lightDir = vPosLight - v.xyz;
		float dist = length(lightDir);
	
		att = 1.0 / ( gl_LightSource[i].constantAttenuation+
			gl_LightSource[i].linearAttenuation*dist+
			gl_LightSource[i].quadraticAttenuation * dist * dist);
	}	

	Idiff = calcLightingDiffuse( n, v, w, vPosLight, vLightDiffuse, bUseFrontMaterialDiffuse, bUseBackMaterialDiffuse ) * att;
	//Iamb = gl_FrontLightProduct[i].ambient;
	Iamb = gl_LightSource[i].ambient;

	vec3 o;
	
	o.r = Idiff.r + Iamb.r;
	o.g = Idiff.g + Iamb.g;
	o.b = Idiff.b + Iamb.b;

	return o;
}

vec3 calcLightingIntensityOpenGL_new( 
	int i,							// OpenGL light number
	vec4 v,							// position of vertex in camera-space
	vec3 n,							// normalized normal of vertex
	vec3 lightDir					// light direction
)
{
	float NdotL = max( dot(n,lightDir), 0.0 );
	
	vec4 color = vec4( 0, 0, 0, 0 );
	
	if ( NdotL > 0.0 )
	{
		color =  gl_LightSource[i].diffuse * NdotL;
	}
	
	color += gl_LightSource[i].ambient;

	return color;
}

vec4 calcLightingAmbientIntensityOpenGL()
{
	return gl_LightModel.ambient;
	//return vec4( 0.0, 0.0, 0.0, 0.0 );
}
