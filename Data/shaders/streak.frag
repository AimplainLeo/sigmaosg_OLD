// streak.frag

uniform sampler2D	tex;
uniform float offsetFactor;
uniform float glowFactor;

void main( void )
{
	float viewport_inv_width = 1.0/800.0;
	float viewport_inv_height = 1.0/600.0;
	float blurFactor = 0.96;

	const vec4 samples_tmp[4] = {
		0.0, -0.0, 0, 0,
		1.0, -1.0, 0, 1,
		2.0, -2.0, 0, 2,
		3.0, -3.0, 0, 3
	};

	const vec4 samples[16] = {
		0.0, -0.0, 0, 0,
		1.0, -1.0, 0, 1,
		2.0, -2.0, 0, 2,
		3.0, -3.0, 0, 3,

		0.0, -0.0, 0, 0,
		-1.0, -1.0, 0, 1,
		-2.0, -2.0, 0, 2,
		-3.0, -3.0, 0, 3,

		0.0, -0.0, 0, 0,
		-1.0, 1.0, 0, 1,
		-2.0, 2.0, 0, 2,
		-3.0, 3.0, 0, 3,

		0.0, 0.0, 0, 0,
		1.0, 1.0, 0, 1,
		2.0, 2.0, 0, 2,
		3.0, 3.0, 0, 3
	};


	vec4 color = vec4( 0,0,0,0 );

	for ( int i=0; i<16; i++ )
	{
		vec4 col = pow( 
						blurFactor, 
						offsetFactor*samples[i].w
					   ) * texture2D( 
							tex, 
							gl_TexCoord[0].xy+
							offsetFactor*vec2(
									samples[i].x*viewport_inv_width, 
									samples[i].y*viewport_inv_height
								 )
					   );
		color += col;				
	}

	gl_FragColor = color*glowFactor;

/*
	float Glow_Factor = 0.1;
	gl_FragColor = min(1.0,
					vec4(color.xyz*Glow_Factor, 0.15)
					);
*/
}
