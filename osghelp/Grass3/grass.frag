// grass.frag

uniform sampler2D tex;

/*
vec4 colorme()
{
	return vec4( abs(testme.x), abs(testme.y), 0.0, 1.0 );
}
*/

void main()
{
	vec4 color = texture2D(tex,gl_TexCoord[0].st);
//	vec4 color = colorme();
	
	gl_FragColor = color;
}
