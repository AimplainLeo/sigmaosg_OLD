// main.frag

uniform sampler2D tex;

vec4 clouds_process();

void main()
{
	//vec4 frag = clouds_process();
	//vec4 frag = texture2D( tex, gl_TexCoord[0] );
	//gl_FragColor = frag;
	
	
	vec4 frag = texture2D( tex, gl_TexCoord[0] );
	
	vec2 st = gl_TexCoord[1].st;
	vec2 vLen = st-vec2(0.5,0.5);
	float len = 1.0-length( vLen )*2.0;
	
	gl_FragColor = vec4( frag.r, frag.g, frag.b, frag.a*len ) ;	
}