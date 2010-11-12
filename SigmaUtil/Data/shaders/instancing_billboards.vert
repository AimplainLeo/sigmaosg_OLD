// instancing_billboards.vert

#extension GL_EXT_gpu_shader4 : enable

// not to be used in this version
/*
uniform int			cc;			
uniform float		spacing;
uniform float		heightAdjust;
*/

// osg devlievered uniforms
uniform float		osg_FrameTime;
uniform mat4		osg_ViewMatrixInverse;
uniform mat4		osg_ViewMatrix;

// our uniforms
uniform float		windFactor;
uniform float		grassStretch;
uniform int			numInstances;
uniform sampler2D	texPositions;
uniform int			texSizeSquared;
uniform int			useLights;

uniform float		minSize;
uniform float		maxSize;

// values to be varying (used in pixel shader)
varying vec4		v;
varying vec3		n;
varying vec4		colorme;

float IntNoise1( int x )
{
	int iSeed = 1000;
	
	x = iSeed+x*57;

	int n = x;
	n =	(n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}

/*
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
	n = colorHeight.xyz;
	float zzz = colorHeight.a;

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

	v = gl_ModelViewMatrix * mV * vec4( vertex, 1.0 );  
}
*/

vec4 CalcLightingDiffuse( vec3 n, vec4 v, const float w, const vec3 vPosLight, const vec4 vLightDiffuse, const vec4 vFrontDiffuse )
{
	// calc vector from vertex to light (all in camera-space)
	vec3 vv = vec3(0.0,0.0,0.0);
	if ( w>0.0 )
	{
		vv = v.xyz;
	}
	vec3 L = normalize( vPosLight-vv ); 
	
	//calculate Diffuse Term:
	return vLightDiffuse * vFrontDiffuse * max(dot(n,L), 0.0);
}

void test()
{
	float inst = float(gl_InstanceID);
	float x = 256.0 / mod( inst, 256.0 );
	float y = inst/256.0;
	vec2 texPos = vec2( x, y );
	vec4 plant_pos = texture2D( texPositions, texPos );

	// x : left right
	// y : back forward
	// z : up down
	
	vec4 vW = osg_ViewMatrixInverse * gl_ModelViewMatrix * gl_Vertex;

/*
	float count = cc;							// this should be a uniform cc^2 instances
	const float r = gl_InstanceID / count;

	// calc placement in ranges 0-1 for both x and y
	vec2 pos2;
	pos2.x = fract( r );
	pos2.y = floor( r ) / count;

	// add random displacement (why the vW here?)
	pos2.x = mod( vW.x+pos2.x+IntNoise1( gl_InstanceID ), 1.0 );
	pos2.y = mod( vW.y+pos2.y+IntNoise1( gl_InstanceID ), 1.0 );

	pos2.x -= 0.5;
	pos2.y -= 0.5;

	float xxx = 0.0;
	float yyy = 0.0;


	vec4 colorHeight = texture2D(texHeightMap,vec2( texPos );
	n = colorHeight.xyz;
	float zzz = colorHeight.a;
*/

	// FIXME: to be replaced when z values are in the position texture
	float xxx = 0.0;
	float yyy = 0.0;
	float zzz = 0.0;

	// move top part of vertices (wind simulations and deforming of quad)
	if ( gl_Vertex.z!=0.0 )
	{
		// wind simulation
		xxx += cos( osg_FrameTime*windFactor+gl_InstanceID )*grassStretch;
		yyy += sin( osg_FrameTime*windFactor+gl_InstanceID )*grassStretch;
		
		// random height adjustment
		zzz = zzz - IntNoise1( gl_InstanceID )*0.3;
	}

	vec4 pos = vec4( plant_pos.x+xxx, plant_pos.y+yyy, plant_pos.z+zzz, 1.0 );
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

	v = gl_ModelViewMatrix * mV * vec4( vertex, 1.0 );  
}

void test2()
{
	float inst = float(gl_InstanceID);
	float x = mod( inst, 256.0 ) / 256.0;
	float y = ( inst/256.0 ) / 256.0;
	vec2 texPos = vec2( x, y );
	vec4 plant_pos = texture2D( texPositions, texPos );

	float xxx = 0.0;
	float yyy = 0.0;
	float zzz = 0.0;

	vec4 pos = vec4( plant_pos.x, plant_pos.y, plant_pos.z+zzz, 1.0 );
	vec4 newX = vec4( 1.0, 0.0, 0.0, 0.0 );
	vec4 newY = vec4( 0.0, 1.0, 0.0, 0.0 );
	vec4 newZ = vec4( 0.0, 0.0, 1.0, 0.0 );
	mat4 mV = mat4( newX, newY, newZ, pos );

	gl_Position = gl_ModelViewProjectionMatrix * mV * gl_Vertex;

	v = gl_ModelViewMatrix * mV * gl_Vertex;  
}

void test3()
{
	float inst = float(gl_InstanceID);

	float x = mod( inst, float(texSizeSquared) ) / float(texSizeSquared);
	float y = ( inst/float(texSizeSquared) ) / float(texSizeSquared);
	vec2 texPos = vec2( x, y );
	vec4 plant_pos = texture2D( texPositions, texPos );

	float xxx = 0.0;
	float yyy = 0.0;
	float zzz = 0.0;

	vec4 pos = vec4( plant_pos.x, plant_pos.y, plant_pos.z+zzz, 1.0 );
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

	v = gl_ModelViewMatrix * mV * gl_Vertex;  
}

void test4()
{
	float inst = float(gl_InstanceID);

	float x = mod( inst, float(texSizeSquared) ) / float(texSizeSquared);
	float y = ( inst/float(texSizeSquared) ) / float(texSizeSquared);
	vec2 texPos = vec2( x, y );
	vec4 plant_pos = texture2D( texPositions, texPos );

	float xxx = 0.0;
	float yyy = 0.0;
	float zzz = 0.0;

	vec4 pos = vec4( plant_pos.x, plant_pos.y, plant_pos.z+zzz, 1.0 );
	vec4 newX = vec4( 1.0, 0.0, 0.0, 0.0 );
	vec4 newY = vec4( 0.0, 1.0, 0.0, 0.0 );
	vec4 newZ = vec4( 0.0, 0.0, 1.0, 0.0 );
	mat4 mV = mat4( newX, newY, newZ, pos );

	// orientation (billboard)
	vec3 X = vec3( gl_ModelViewMatrix[0][0], gl_ModelViewMatrix[1][0], gl_ModelViewMatrix[2][0] );
    vec3 Y = vec3( gl_ModelViewMatrix[0][1], gl_ModelViewMatrix[1][1], gl_ModelViewMatrix[2][1] );
    vec3 Z = vec3( gl_ModelViewMatrix[0][2], gl_ModelViewMatrix[1][2], gl_ModelViewMatrix[2][2] );
		
	vec4 glv = gl_Vertex;
	//float scale = (1.0+noise1(inst/float(numInstances))/2.0)*10.0;
	//float scale = (1.0+IntNoise1(gl_InstanceID)/2.0)*10.0;
	float scale = (1.0+IntNoise1(gl_InstanceID)/2.0)*(maxSize-minSize)+minSize;

	glv.x = glv.x*scale;
	glv.y = glv.y*scale;
	glv.z = glv.z*scale;

	vec3 xx = glv.x*X;
	vec3 yy = glv.y*Y;
	xx.z = 0.0;
	yy.z = 0.0;
	vec3 vertex = xx + yy + vec3( 0.0, 0.0, glv.z );


	gl_Position = gl_ModelViewProjectionMatrix * mV * vec4( vertex, 1.0 );

	v = gl_ModelViewMatrix * mV * vec4( vertex, 1.0 );
}

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

	float f = mod( float(gl_InstanceID), 8.0 );
	gl_TexCoord[0].x = f/8.0f + (gl_MultiTexCoord0.x/8.0);

	//simple2();
	//test3();
	test4();

	gl_FrontColor = gl_Color;

	/*
	mat3 worldView = mat3(osg_ViewMatrix);
	vec3 nn = worldView * vec3(0,0,1);

	vec4 vFrontDiffuse = vec4(1,1,1,1);
	colorme = CalcLightingDiffuse( 
		nn, v, 
		gl_LightSource[3].position.w,
		gl_LightSource[3].position.xyz,
		gl_LightSource[3].diffuse,
		vFrontDiffuse );
	colorme.a = 1.0;
	*/

	colorme.r = 0.0;
	colorme.g = 0.0;
	colorme.b = 0.0;
	colorme.a = 1.0;
	if ( useLights & 0x00000001 ) colorme += gl_LightSource[0].diffuse + gl_LightSource[0].ambient;
	if ( useLights & 0x00000002 ) colorme += gl_LightSource[1].diffuse + gl_LightSource[1].ambient;
	if ( useLights & 0x00000004 ) colorme += gl_LightSource[2].diffuse + gl_LightSource[2].ambient;
	if ( useLights & 0x00000008 ) colorme += gl_LightSource[3].diffuse + gl_LightSource[3].ambient;
	
	colorme.a = 1.0;
/*
	// sun light
	colorme = gl_LightSource[3].diffuse + gl_LightSource[3].ambient;
	// moon light
	colorme += gl_LightSource[2].diffuse + gl_LightSource[2].ambient;
	colorme.a = 1.0;
	//colorme = vec4(0.5, 0.5, 0.5, 1.0);
*/
}
