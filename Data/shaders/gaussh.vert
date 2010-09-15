// gaussh.vert

varying vec2 texCoord;

void main(void)
{
/*
	vec2 P = sign( gl_Vertex.xy );
	gl_Position = vec4( P, 0.0, 1.0 );
	texCoord = P * 0.5 + 0.5;
	gl_Position = ftransform();
	*/

	vec2 P = sign( gl_Vertex.xy );
	texCoord = P;
	gl_Position = ftransform();	
}