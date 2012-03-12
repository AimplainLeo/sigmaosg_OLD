// funcTree.vert

// instancing_billboards.vert

#extension GL_EXT_gpu_shader4 : enable

uniform float colorAmpFactor;

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
//varying vec4		colorme;
//varying float		fogFactor;

float IntNoise1( int x )
{
	int iSeed = 1000;
	
	x = iSeed+x*57;

	int n = x;
	n =	(n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}

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

// returns the adjusted object-space position for this vertex
vec4 test4()
{
	float inst = float(gl_InstanceID);

	float x = mod( inst, float(texSizeSquared) ) / float(texSizeSquared);
	float y = ( inst/float(texSizeSquared) ) / float(texSizeSquared);
	vec2 texPos = vec2( x, y );
	vec4 plant_pos = texture2D( texPositions, texPos );

	vec4 pos = vec4( plant_pos.x, plant_pos.y, plant_pos.z, 1.0 );
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

	return mV * vec4( vertex, 1.0 );
}

vec4 lightContribution(in int lightNum, in vec4 vsPos)
{
	/* first transform the normal into eye space and normalize the result */
	vec3 normal = normalize(gl_NormalMatrix * gl_Normal);

	float intensity = 1.0;
	if(gl_LightSource[lightNum].position.w == 0.0)
	{
		// directional light
		vec3 lightDir;
		lightDir = normalize(vec3(gl_LightSource[lightNum].position));
		intensity = max(dot(normal, lightDir), 0.0);
	}
	else
	{
		// point light
		vec3 vsPointToLight = vec3(gl_LightSource[lightNum].position - vsPos);
		float dist = length(vsPointToLight);
		vec3 lightDir = normalize(vsPointToLight);
		float NdotL = max(dot(normal, lightDir), 0.0);
		float attenuation = 1.0 / (gl_LightSource[lightNum].constantAttenuation +
					gl_LightSource[lightNum].linearAttenuation * dist +
					gl_LightSource[lightNum].quadraticAttenuation * dist * dist);
		intensity = attenuation * NdotL;
	}

	/* Compute the diffuse term */
	return intensity * gl_LightSource[lightNum].diffuse + gl_FrontLightProduct[lightNum].ambient;
}

float getFogFactor( in vec4 vsPos )
{
	float z = -vsPos.z;
	return clamp((gl_Fog.end - z) * gl_Fog.scale, 0.0, 1.0);
}

vec4 calcTree()
{
//	gl_TexCoord[0] = gl_MultiTexCoord0;

	float f = mod( float(gl_InstanceID), 8.0 );
	gl_TexCoord[0].x = f/8.0 + (gl_MultiTexCoord0.x/8.0);

	vec4 osPosition = test4(); // test4 computes the object-space position of the current vertex

	gl_Position = gl_ModelViewProjectionMatrix * osPosition;
	vec4 vsPosition = gl_ModelViewMatrix * osPosition;
	return vsPosition;

//	gl_FrontColor = gl_Color;

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

/*
	colorme.r = 0.0;
	colorme.g = 0.0;
	colorme.b = 0.0;
	colorme.a = 1.0;
	if ( (useLights & 0x00000001) != 0 ) colorme += lightContribution(0, vsPosition);
	if ( (useLights & 0x00000002) != 0 ) colorme += lightContribution(1, vsPosition);
	if ( (useLights & 0x00000004) != 0 ) colorme += lightContribution(2, vsPosition);
	if ( (useLights & 0x00000008) != 0 ) colorme += lightContribution(3, vsPosition);
	
	colorme = colorme*colorAmpFactor;
	colorme.a = 1.0;
	fogFactor = getFogFactor(vsPosition);
	*/
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

// returns the adjusted object-space position for this vertex
vec4 test_non_billboard()
{
	float inst = float(gl_InstanceID);

	float x = mod( inst, float(texSizeSquared) ) / float(texSizeSquared);
	float y = ( inst/float(texSizeSquared) ) / float(texSizeSquared);
	vec2 texPos = vec2( x, y );
	vec4 plant_pos = texture2D( texPositions, texPos );

	// scale original vertex		
	float scale = (1.0+IntNoise1(gl_InstanceID)/2.0)*(maxSize-minSize)+minSize;
	return plant_pos + vec4( gl_Vertex.xyz*scale, 0.0 );
}

vec4 calcTree2()
{
	float f = mod( float(gl_InstanceID), 8.0 );
	gl_TexCoord[0].x = f/8.0 + (gl_MultiTexCoord0.x/8.0);

	vec4 osPosition = test_non_billboard(); 

	gl_Position = gl_ModelViewProjectionMatrix * osPosition;
	vec4 vsPosition = gl_ModelViewMatrix * osPosition;
	return vsPosition;
}
