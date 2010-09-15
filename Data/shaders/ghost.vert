// ghost.vert

varying vec2 texCoord1;
varying vec2 texCoord2;
varying vec2 texCoord3;
varying vec2 texCoord4;

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

	vec2 texCoord = gl_MultiTexCoord0.xy;
	texCoord1 = (texCoord-0.5)*(-2.0) + 0.5;
	texCoord2 = (texCoord-0.5)*(2.0) + 0.5;
	texCoord3 = (texCoord-0.5)*(-0.6) + 0.5;
	texCoord4 = (texCoord-0.5)*(0.6) + 0.5;
	
	gl_Position = ftransform();
}

