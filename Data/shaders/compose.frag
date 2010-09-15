// compose.frag

uniform sampler2D tex;
uniform sampler2D bloom;

uniform float exposure;
uniform float bloomFactor;
uniform float brightMax;

void main()
{
	vec2 st = gl_TexCoord[0].st;
	vec4 color = texture2D( tex, st );
	vec4 colorBloom = texture2D( bloom, st );
	
	color += colorBloom*bloomFactor;
	
	// perform tone-mapping
	float Y = dot(vec4(0.30, 0.59, 0.11, 0.0), color );
	float YD = exposure*(exposure/brightMax+1.0) / (exposure+1.0);
	color *= YD;
	
	gl_FragColor = color;
}