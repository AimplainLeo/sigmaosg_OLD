// bw.frag

uniform bool		bInvert;
uniform sampler2D	tex;
uniform float		osg_FrameTime;
uniform bool		bNoise;

float rand( vec2 co )
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main( void )
{
	vec4 frag;
    vec4 texelColor = texture2D( tex, gl_TexCoord[0].xy );
    vec4 grayScaleWeights = vec4(0.3, 0.59, 0.11, 0.0);
    vec4 scaledColor = texelColor * grayScaleWeights;
    float luminance = scaledColor.r + scaledColor.g + scaledColor.b;
    
    if ( bInvert==true )
    {
		frag = vec4( 1.0-luminance, 1.0-luminance, 1.0-luminance, texelColor.a );
	}
	else
	{
		frag = vec4( luminance, luminance, luminance, texelColor.a );
	}

	if ( bNoise )
	{	
		frag = frag + 
			vec4( 
				rand( gl_TexCoord[0].xy*osg_FrameTime ),
				rand( gl_TexCoord[0].xy*osg_FrameTime ),
				rand( gl_TexCoord[0].xy*osg_FrameTime ),
				1.0
			);
			
		frag.r = clamp( frag.r, 0.0, 1.0 );
		frag.g = clamp( frag.g, 0.0, 1.0 );
		frag.b = clamp( frag.b, 0.0, 1.0 );
	}
	
	//frag = vec4( 1,0,0,1 );
	
	gl_FragColor = frag;
}
