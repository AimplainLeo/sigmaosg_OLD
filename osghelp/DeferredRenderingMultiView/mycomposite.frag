// mycomposite.frag

uniform sampler2D texPositions;
uniform sampler2D texNormals;
uniform sampler2D texColors;

varying vec3 lightDir2;
varying vec3 lightDir3;

vec4 sulCalcLightPoint( vec4 v, vec3 n, bool ignoreNormal );
vec4 sulCalcLightSpot( vec4 v, vec3 n, bool ignoreNormal );

vec3 calcLightingIntensityOpenGL_new( 
	int i,							// OpenGL light number
	vec4 v,							// position of vertex in camera-space
	vec3 n,							// normalized normal of vertex
	vec3 lightDir					// light direction
)
{
	float NdotL = max( dot(n,lightDir), 0.0 );
	
	vec4 color = vec4( 0, 0, 0, 0 );
	
	if ( NdotL > 0.0 )
	{
		color =  gl_LightSource[i].diffuse * NdotL;
	}
	
	color += gl_LightSource[i].ambient;

	return color;
}

// remember we are rending to a quad here!
void main( void )
{
	vec2 st = gl_TexCoord[0].st;
	vec4 tPos		= texture2D( texPositions, st );
	vec4 tNormals	= texture2D( texNormals, st );
	vec4 tColors	= texture2D( texColors, st );
	
	vec4 frag;
	
//	frag = tColors * (sulCalcLightPoint( tPos, tNormals.xyz, false )+sulCalcLightSpot( tPos, tNormals.xyz, false ));

	vec4 iLS = sulCalcLightSpot( tPos, tNormals.xyz, false );
	vec4 iLP = sulCalcLightPoint( tPos, tNormals.xyz, false );
	vec4 iOpenGL_light2 = vec4( calcLightingIntensityOpenGL_new( 2, tPos, tNormals.xyz, lightDir2 ), 1.0 );
	vec4 iOpenGL_light3 = vec4( calcLightingIntensityOpenGL_new( 3, tPos, tNormals.xyz, lightDir3 ), 1.0 );
	frag = tColors * (iLP+iLS+iOpenGL_light2+iOpenGL_light3);

	gl_FragColor = frag;	
}