// z.vert

varying vec3 pos;

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
	pos = gl_Position;
}

