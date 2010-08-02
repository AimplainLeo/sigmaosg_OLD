// gui_frame.frag

uniform sampler2D	tex;

uniform float w;
uniform float h;
uniform float border;
uniform vec4 bg_color;
uniform vec4 border_color;

void main( void )
{
	// calc uv to pixel
	float px = gl_TexCoord[0].x * w;
	float py = gl_TexCoord[0].y * h;

	vec4 color = bg_color;

	if ( px<border || px>(w-border)  || py<border || py>(h-border) )
	{
		color = border_color;
	}

	gl_FragColor = color;
}
