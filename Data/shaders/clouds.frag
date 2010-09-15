// clouds.frag

uniform sampler2D tex;

vec4 clouds_process()
{
	vec4 frag = texture2D( tex, gl_TexCoord[0] );

	vec2 st = gl_TexCoord[1].st;
	vec2 vLen = st-vec2(0.5,0.5);
	float len = 1.0-length( vLen )*2.0;

	vec4 fragOut = vec4( frag.a, frag.a, frag.a, frag.a );

	fragOut.a = fragOut.a*len;

	return fragOut;
}

/*
vec4 CalcFogExp2( vec4 fragIn )
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
	fragOut = mix( gl_Fog.color, fragIn, fogFactor );

	return fragOut;
}

vec4 CalcFogLinear( vec4 fragIn )
{
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float fogFactor = (gl_Fog.end - z) * gl_Fog.scale;	
	fogFactor = clamp( fogFactor, 0.0, 1.0 );

	vec4 fragOut;
	fragOut = mix( gl_Fog.color, fragIn, fogFactor );

	return fragOut;
}
*/




