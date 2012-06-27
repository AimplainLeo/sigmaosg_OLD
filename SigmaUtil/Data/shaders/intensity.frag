// intensity.frag

uniform sampler2D tex;
uniform float multiR;
uniform float multiG;
uniform float multiB;
uniform float intensity;

uniform bool bInvert;

void main( void )
{
	vec4 texelColor = texture2D( tex, gl_TexCoord[0].xy );
    vec4 grayScaleWeights = vec4(0.3, 0.59, 0.11, 0.0);
    vec4 scaledColor = texelColor * grayScaleWeights;
    float luminance = scaledColor.r + scaledColor.g + scaledColor.b;
    
    luminance = luminance*intensity;
    
	if ( bInvert )
		gl_FragColor = vec4( 1.0 - luminance*multiR, 1.0 - luminance*multiG, 1.0 - luminance*multiB, texelColor.a );
	else
		gl_FragColor = vec4( luminance*multiR, luminance*multiG, luminance*multiB, texelColor.a );
}
