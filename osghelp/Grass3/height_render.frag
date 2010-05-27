// height_render.frag

uniform sampler2D tex;
varying vec4 pos_world;

void main()
{
	gl_FragColor = vec4( pos_world.z, pos_world.z, pos_world.z, 1.0 );
	//gl_FragColor =  vec4( 1.0, 0.0, 0.0, 1.0 );
	//gl_FragColor =  vec4( 0.6, 0.6, 0.6, 0.6 );
	//gl_FragColor = texture2D(tex,gl_TexCoord[0].st);
}