// z.frag

varying vec3 pos;

void main( void )
{
	gl_FragColor = vec4( pos.z, pos.z, pos.z, pos.z );
}
