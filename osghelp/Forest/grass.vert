// grass.vert

uniform int			cc;
uniform float		spacing;
uniform float		osg_SimulationTime;
uniform float		osg_FrameTime;
uniform mat4		osg_ViewMatrixInverse;
uniform sampler2D	texHeightMap;
uniform float		heightAdjust;
uniform float		windFactor;
uniform float		grassStretch;
uniform bool		bReceiveShadow;

float IntNoise1( int x )
{
	int iSeed = 1000;
	
	x = iSeed+x*57;

	int n = x;
	n =	(n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}

void DynamicShadow( in vec4 ecPosition )
{
    // generate coords for shadow mapping                          
    gl_TexCoord[1].s = dot( ecPosition, gl_EyePlaneS[1] );
    gl_TexCoord[1].t = dot( ecPosition, gl_EyePlaneT[1] );
    gl_TexCoord[1].p = dot( ecPosition, gl_EyePlaneR[1] );
    gl_TexCoord[1].q = dot( ecPosition, gl_EyePlaneQ[1] );
}

void simple2()
{
	// x : left right
	// y : back forward
	// z : up down
	
	vec4 vW = osg_ViewMatrixInverse * gl_ModelViewMatrix * gl_Vertex;

	float count = cc;							// this should be a uniform cc^2 instances
	const float r = gl_InstanceID / count;
	float len = count*spacing; 					// length of side of grid

	// calc placement in ranges 0-1 for both x and y
	vec2 pos2;
	pos2.x = fract( r );
	pos2.y = floor( r ) / count;


	// add random displacement
	pos2.x = mod( vW.x+pos2.x+IntNoise1( gl_InstanceID ), 1.0 );
	pos2.y = mod( vW.y+pos2.y+IntNoise1( gl_InstanceID ), 1.0 );

	pos2.x -= 0.5;
	pos2.y -= 0.5;

	float xxx = 0.0;
	float yyy = 0.0;

	vec4 colorHeight = texture2D(texHeightMap,vec2( (pos2.y+0.5), 1.0-(pos2.x+0.5)));
	float zzz = colorHeight.r;

	// move top part of vertices (wind simulations and deforming of quad)
	if ( gl_Vertex.z!=0.0 )
	{
		// wind simulation
		xxx += cos( osg_FrameTime*windFactor+gl_InstanceID )*grassStretch;
		yyy += sin( osg_FrameTime*windFactor+gl_InstanceID )*grassStretch;
		
		// random height adjustment
		zzz = zzz - IntNoise1( gl_InstanceID )*0.3;
		
		// height multiplier
		zzz = zzz+heightAdjust;
	}

	vec4 pos = vec4( pos2.x*len+xxx, pos2.y*len+yyy, zzz, 1.0 );
	vec4 newX = vec4( 1.0, 0.0, 0.0, 0.0 );
	vec4 newY = vec4( 0.0, 1.0, 0.0, 0.0 );
	vec4 newZ = vec4( 0.0, 0.0, 1.0, 0.0 );
	mat4 mV = mat4( newX, newY, newZ, pos );

	// orientation (billboard)
	vec3 X = vec3( gl_ModelViewMatrix[0][0], gl_ModelViewMatrix[1][0], gl_ModelViewMatrix[2][0] );
    vec3 Y = vec3( gl_ModelViewMatrix[0][1], gl_ModelViewMatrix[1][1], gl_ModelViewMatrix[2][1] );
    vec3 Z = vec3( gl_ModelViewMatrix[0][2], gl_ModelViewMatrix[1][2], gl_ModelViewMatrix[2][2] );
		
	vec3 xx = gl_Vertex.x*X;
	vec3 yy = gl_Vertex.y*Y;
	xx.z = 0.0;
	yy.z = 0.0;
	vec3 vertex = xx + yy + vec3( 0.0, 0.0, gl_Vertex.z );

	gl_Position = gl_ModelViewProjectionMatrix * mV * vec4( vertex, 1.0 );
	
	if ( bReceiveShadow )
	{
		vec4  ecPos  = gl_ModelViewMatrix * mV * vec4( vertex, 1.0 );  
		DynamicShadow( ecPos );
	}
}

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

//	vec4  ecPos  = gl_ModelViewMatrix * gl_Vertex;  
//	DynamicShadow( ecPos );

	simple2();

	gl_FrontColor = gl_Color;

//	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
}

	