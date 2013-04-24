// mycomposite.frag

uniform sampler2D texPositions;
uniform sampler2D texNormals;
uniform sampler2D texColors;

vec4 sulCalcLightPoint( vec4 v, vec3 n, bool ignoreNormal );

// remember we are rending to a quad here!
void main( void )
{
	vec2 st = gl_TexCoord[0].st;
	vec4 tPos		= texture2D( texPositions, st );
	vec4 tNormals	= texture2D( texNormals, st );
	vec4 tColors	= texture2D( texColors, st );
	
	vec4 frag;
	
	frag = tColors * sulCalcLightPoint( tPos, tNormals.xyz, false );

	gl_FragColor = frag;	
}