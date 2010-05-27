// bw.frag

uniform bool		bInvert;
uniform sampler2D	tex;

void main( void )
{

    vec4 texelColor = texture2D( tex, gl_TexCoord[0].xy );
    vec4 grayScaleWeights = vec4(0.3, 0.59, 0.11, 0.0);
    vec4 scaledColor = texelColor * grayScaleWeights;
    float luminance = scaledColor.r + scaledColor.g + scaledColor.b;
    
    if ( bInvert==true )
    {
		gl_FragColor = vec4( 1.0-luminance, 1.0-luminance, 1.0-luminance, texelColor.a );
	}
	else
	{
		gl_FragColor = vec4( luminance, luminance, luminance, texelColor.a );
	}
	
	//gl_FragColor = vec4( 1,0,0,1 );
}
