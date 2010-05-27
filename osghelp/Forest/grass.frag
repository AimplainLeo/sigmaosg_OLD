// grass.frag

uniform sampler2D tex;

uniform sampler2DShadow osgShadow_shadowTexture;
uniform vec2 osgShadow_ambientBias;
uniform sampler2D osgShadow_baseTexture;
uniform bool		bReceiveShadow;

vec4 CalcFog( vec4 fragIn )
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

void main()
{
	vec4 color = texture2D( tex, gl_TexCoord[0].st );

	if ( bReceiveShadow )
	{
		vec4 shadow = shadow2DProj( osgShadow_shadowTexture, gl_TexCoord[1] );
		if ( shadow.r<0.9 )
		{
			color.rgb = color.rgb * 0.7;
		}
	}

	gl_FragColor = color;
}