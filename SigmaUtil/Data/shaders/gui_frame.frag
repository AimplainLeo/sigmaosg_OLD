// gui_frame.frag

uniform sampler2D	tex;

uniform float w;
uniform float h;
uniform float border;

void main( void )
{
	// calc uv to pixel
	float px = gl_TexCoord[0].x * w;
	float py = gl_TexCoord[0].y * h;

	vec4 color = vec4( 0, 0, 0, 0.2f );
/*
	if ( px<border || py<border )
	{
		color = vec4( 0,0,1,1 );
	}

	if ( px>(w-border) || py>(h-border) )
	{
		color = vec4( 0,0,0.7,1 );
	}
*/

	if ( px<border || px>(w-border)  || py<border || py>(h-border) )
	{
		color = vec4( 0,0,1,1 );
	}

	gl_FragColor = color;
}
