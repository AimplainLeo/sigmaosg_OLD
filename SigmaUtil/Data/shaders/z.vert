// z.vert


varying vec3 pos;

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	pos = gl_ModelViewMatrix*gl_Vertex;

	gl_Position = ftransform();
}

