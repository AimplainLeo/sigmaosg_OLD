// ghost.frag

uniform sampler2D tex;
//uniform float Glow_Factor;

varying vec2 texCoord1;
varying vec2 texCoord2;
varying vec2 texCoord3;
varying vec2 texCoord4;

float fadeOut_org(in vec2 c)
{
	vec2 d = vec2(0.5,0.5) - c;
	float l = length(d);
	return max(0.0, 1.0 - 2.0 * l);
}

float fadeOut(in vec2 c)
{
	vec2 d = vec2(0.5,0.5) - c;
	float l = length(d);
	return max(0.0, 1.0 - 2.0 * l);
}

void main()
{
	vec4 col1 = texture2D(tex, texCoord1) * fadeOut(texCoord1);
	vec4 col2 = texture2D(tex, texCoord2) * fadeOut(texCoord2);
	vec4 col3 = texture2D(tex, texCoord3) * fadeOut(texCoord3);
	vec4 col4 = texture2D(tex, texCoord4) * fadeOut(texCoord4);

	float Glow_Factor = 0.2;
	gl_FragColor = (col1 + col2 + col3 + col4) * Glow_Factor;
}