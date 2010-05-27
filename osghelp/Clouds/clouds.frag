// clouds.frag

uniform sampler2D tex;

void main()
{
	
	vec4 frag = texture2D( tex, gl_TexCoord[0] );
	
	vec2 st = gl_TexCoord[1].st;
	vec2 vLen = st-vec2(0.5,0.5);
	float len = 1.0-length( vLen )*2.0;
	
	gl_FragColor = vec4( frag.a, frag.a, frag.a, frag.a*len ) ;
}
