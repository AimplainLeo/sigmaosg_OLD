// gui_frame.frag

uniform sampler2D	tex;

uniform float w;
uniform float h;
uniform float border;
uniform vec4 bg_color;
uniform vec4 border_color;
uniform int cover;

uniform sampler2D tex0;
uniform int use_texture;

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

	if ( cover )
	{
		gl_FragColor = vec4(0.5,0.5,0.5,1) + color;
	}

	if ( use_texture )
	{
		vec2 st = gl_TexCoord[0].st;
		st[1] = 1.0-st[1];
		vec4 color0 = texture2D( tex0, st );
		gl_FragColor = color0;
	}
}
