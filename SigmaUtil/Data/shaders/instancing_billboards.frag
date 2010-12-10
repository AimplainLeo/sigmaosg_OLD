// instancing_billboards.frag

uniform sampler2D			tex;

varying vec4				v;
varying vec3				n;
varying vec4				colorme;
varying float				fogFactor;


void main()
{
	vec4 frag = texture2D( tex, gl_TexCoord[0].st );

	frag = mix(gl_Fog.color, colorme*frag, fogFactor);

	gl_FragColor = frag;
}