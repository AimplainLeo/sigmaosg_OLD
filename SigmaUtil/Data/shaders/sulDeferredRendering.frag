// sulDefferedRendering.frag

uniform sampler2D		tex;

varying vec3			n;
varying vec4			v;

void main( void )
{
	vec3 nn = normalize( n );
	
	gl_FragData[0] = v;
	gl_FragData[1] = vec4( nn, 1.0 );
	gl_FragData[2] = gl_Color;
	//gl_FragData[2] = texture2D( tex, gl_TexCoord[0].st );
}