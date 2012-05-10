// myshader.frag

varying vec3			n;
varying vec4			v;

// prototypes
vec4 sulCalcLightPoint( vec4 v, vec3 n );

void main( void )
{
	vec4 frag;

	frag = gl_Color;
	
	vec3 nn = normalize( n );
	
	vec4 iDDD = sulCalcLightPoint( v, nn );
	
	frag = frag * iDDD;
	
	frag.a = 1.0;
	
	frag.b = 1.0;
	
	gl_FragColor = frag;
}