// terrain.frag

uniform sampler2D tex;
uniform sampler2DShadow osgShadow_shadowTexture;

void main()
{
	vec4 color = texture2D( tex, gl_TexCoord[0].st );

	vec4 shadow = shadow2DProj( osgShadow_shadowTexture, gl_TexCoord[1] );
	if ( shadow.r<0.9 )
	{
		color.rgb = color.rgb * 0.6;
	}

	gl_FragColor = color;
}