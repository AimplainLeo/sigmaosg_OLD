// placement_render.frag

varying vec4 pos_world;

void main()
{
	gl_FragColor = vec4( pos_world.z, pos_world.z, pos_world.z, 1.0 );
}