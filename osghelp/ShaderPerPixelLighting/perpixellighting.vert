varying vec3 N;
varying vec3 v;

void main(void)
{
	// transform vertex from object-space to camera-space
	v = vec3( gl_ModelViewMatrix * gl_Vertex);

	// transform normal from object-space to camera-space
	N = normalize(gl_NormalMatrix * gl_Normal);   
   
   // transform vertex into from object-space to camera-space with projection
   // this is the same as: gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
} 
