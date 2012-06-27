// height_render.frag

uniform sampler2D tex;
varying vec4 pos_world;
varying vec3 norm;

void main()
{
	vec3 n = normalize(norm);
	gl_FragColor = vec4( norm.x, norm.y, norm.z, pos_world.z );
	//gl_FragColor =  vec4( 1.0, 0.0, 0.0, 1.0 );
	//gl_FragColor =  vec4( 0.6, 0.6, 0.6, 0.6 );
	//gl_FragColor = texture2D(tex,gl_TexCoord[0].st);
}