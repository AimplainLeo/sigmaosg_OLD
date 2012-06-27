// myshader.frag

varying vec3			n;
varying vec4			v;

void main( void )
{
	vec4 frag;

	frag = gl_Color;
	
	vec3 nn = normalize( n );
	
	//gl_FragColor = vec4( nn, 1.0 );		// normal
	
		
	//gl_FragColor = v;
	
	gl_FragData[0] = v;
	gl_FragData[1] = vec4( nn, 1.0 );
	gl_FragData[2] = gl_Color;
}