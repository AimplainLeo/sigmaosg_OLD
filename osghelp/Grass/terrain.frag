// terrain.frag

varying vec4 pos_world;
uniform sampler2D tex;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
	vec4 color = texture2D(tex,gl_TexCoord[0].st);

/*	
	vec4 pos = projMat * viewMat * pos_world;
	if ( pos.x>-1.0 && pos.x<1.0 && pos.y>-1.0 && pos.y<1.0 )
		color.r = 1.0;
*/
	
	gl_FragColor = color;
}