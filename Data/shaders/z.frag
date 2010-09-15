// z.frag

varying vec3 pos;

void main( void )
{
	gl_FragColor = vec4( pos.x, pos.y, pos.z, 1.0 );
}
