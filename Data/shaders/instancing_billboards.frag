// instancing_billboards.frag

uniform sampler2D			tex;

varying vec4				v;
varying vec3				n;
varying vec4				colorme;


void main()
{
	vec4 frag = texture2D( tex, gl_TexCoord[0].st );

	frag = colorme*frag;

	gl_FragColor = frag;
}