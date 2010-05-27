// terrain.vert

void DynamicShadow( in vec4 ecPosition )
{
    // generate coords for shadow mapping                          
    gl_TexCoord[1].s = dot( ecPosition, gl_EyePlaneS[1] );
    gl_TexCoord[1].t = dot( ecPosition, gl_EyePlaneT[1] );
    gl_TexCoord[1].p = dot( ecPosition, gl_EyePlaneR[1] );
    gl_TexCoord[1].q = dot( ecPosition, gl_EyePlaneQ[1] );
}

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

	vec4  ecPos  = gl_ModelViewMatrix * gl_Vertex;  
	DynamicShadow( ecPos );

	gl_FrontColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
}
