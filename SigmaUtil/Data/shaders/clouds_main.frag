// clouds_main.frag

// prototypes
vec4 clouds_process();

void main()
{
	vec4 frag = clouds_process();

	/* len is calculated in the clouds_process()
	vec4 temp = CalcFogLinear( fragOut );
	temp.a = temp.a*len;
	*/

	gl_FragColor = temp;
}
