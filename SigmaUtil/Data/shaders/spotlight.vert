// spotlight.vert

varying vec4 v;
varying vec3 n;

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	v = gl_ModelViewMatrix * gl_Vertex;

	// transform normal from object-space to camera-space
	n = gl_NormalMatrix * gl_Normal;   

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}