// compose.frag

uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
	vec4 color0 = texture2D( tex0, gl_TexCoord[0].st );
	vec4 color1 = texture2D( tex1, gl_TexCoord[1].st );

	gl_FragColor = color0+color1;

	//gl_FragColor = color0;
	//gl_FragColor = color1;
}