// brightpass.frag

uniform sampler2D baseImage;
uniform float brightPassThreshold;
uniform float brightPassFalloff;

void main(void) 
{
    vec3 luminanceVector = vec3(0.2125, 0.2154, 0.2721);
    vec4 sample = texture2D(baseImage, gl_TexCoord[0].st);

	float luminance = dot(luminanceVector, sample.rgb);
	if(luminance < brightPassThreshold)
	{
		if(brightPassFalloff < 1e-5)
		{
			sample = vec4(0.0, 0.0, 0.0, 1.0);
		}
		else
		{
			float a = 1.0 / brightPassFalloff;
			float b = 1 - a;
			float s = max(0.0, a * luminance/brightPassThreshold + b);
			sample *= s;
			sample.a = 1.0;
		}
	}

    gl_FragColor = sample;
}

/*
void main(void) 
{
    vec3 luminanceVector = vec3(0.2125, 0.7154, 0.0721);
    vec4 sample = texture2D(baseImage, gl_TexCoord[0].st);

    float luminance = dot(luminanceVector, sample.rgb);
    luminance = max(0.0, luminance - brightPassThreshold);

	if ( luminance>1.0 )
	{
		gl_FragColor = vec4(1.0,0.0,0.0,1.0);
	}
	else
	{
		gl_FragColor = vec4(0.0,0.0,0.0,1.0);
	}
}
*/