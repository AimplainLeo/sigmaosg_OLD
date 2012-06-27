// myshader.vert

varying vec3			n;
varying vec4			v;

void main( void )
{
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;

	// transform vertex from object-space to camera-space
	v = gl_ModelViewMatrix * gl_Vertex;

	// transform normal from object-space to camera-space
	n = normalize( gl_NormalMatrix * gl_Normal );   

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}