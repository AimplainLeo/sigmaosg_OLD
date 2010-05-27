// amplifier.frag

uniform sampler2D tex;
uniform float fAmp;
uniform bool bColor;

void main( void )
{
    vec4 texelColor = texture2D( tex, gl_TexCoord[0].xy );
	
	if ( bColor )
	{
	/*
		texelColor.r = texelColor.r+gl_Color.r*fAmp;
		texelColor.g = texelColor.g+gl_Color.g*fAmp;
		texelColor.b = texelColor.b+gl_Color.b*fAmp;
		texelColor.a = texelColor.a*gl_Color.a;
		gl_FragColor = texelColor;
		*/
		
		gl_FragColor = vec4( gl_Color.r*fAmp, gl_Color.g*fAmp, gl_Color.b*fAmp, gl_Color.a );
	}
	else
	{
		texelColor.r = texelColor.r*fAmp;
		texelColor.g = texelColor.g*fAmp;
		texelColor.b = texelColor.b*fAmp;
		//clamp( texelColor, 0.0, 1.0 );
		gl_FragColor = texelColor;
	}
}
