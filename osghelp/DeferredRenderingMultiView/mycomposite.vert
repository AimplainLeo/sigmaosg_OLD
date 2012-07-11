// mycomposite.vert

varying vec3 lightDir2;
varying vec3 lightDir3;

void main( void )
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	// MOON: directional light
	lightDir2 = normalize( gl_LightSource[2].position.xyz );

	// SUN: directional light
	lightDir3 = normalize( gl_LightSource[3].position.xyz );	
}
