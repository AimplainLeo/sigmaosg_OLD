// placement_render.vert

uniform mat4 osg_ViewMatrixInverse;
varying vec4 pos_world;

uniform mat4 viewInverseMatrixCam;

void main()
{
	//pos_world = viewInverseMatrixCam * gl_ModelViewMatrix * gl_Vertex;
	pos_world = osg_ViewMatrixInverse * gl_ModelViewMatrix * gl_Vertex;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}