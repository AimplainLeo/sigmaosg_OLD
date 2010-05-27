// terrain.vert

uniform mat4 osg_ViewMatrixInverse;
varying vec4 pos_world;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	pos_world		= osg_ViewMatrixInverse * gl_ModelViewMatrix * gl_Vertex;
	
	vec4 pos = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = pos;
}