// funcFogOpenGL.frag

vec4 calcFogLinear( vec4 fragIn )
{
	float s = gl_Fog.scale;

	// NOTE: gl_FogFragCoord doesn't seem to be the same as the calculated z value
	//float fogFactor = (gl_Fog.end - gl_FogFragCoord) * gl_Fog.scale;	
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float fogFactor = (gl_Fog.end - z) * gl_Fog.scale;	

	fogFactor = clamp( fogFactor, 0.0, 1.0 );

	vec4 fragOut;
	fragOut = mix( gl_Fog.color, fragIn, fogFactor );
	fragOut.a = fragIn.a;

	return fragOut;
}

vec4 calcFogExp2( vec4 fragIn )
{
	const float LOG2 = 1.442695;
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float fogFactor = exp2( -gl_Fog.density * 
				   gl_Fog.density * 
				   z * 
				   z * 
				   LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	vec4 fragOut;
	fragOut = mix(gl_Fog.color, fragIn, fogFactor );

	return fragOut;
}