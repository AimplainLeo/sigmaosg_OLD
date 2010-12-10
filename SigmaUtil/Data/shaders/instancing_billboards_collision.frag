// instancing_billboards.frag

uniform sampler2D			tex;

varying vec4				v;
varying vec3				n;
varying vec4				colorme;
varying float				fogFactor;

void main()
{
	vec4 texture = texture2D( tex, gl_TexCoord[0].st );
	gl_FragData[0] = mix( gl_Fog.color, colorme*texture, fogFactor );
	gl_FragData[0].a = texture.a;
	gl_FragData[1] = vec4( -v.z, 0.0, 0.0, 1.0 );
}