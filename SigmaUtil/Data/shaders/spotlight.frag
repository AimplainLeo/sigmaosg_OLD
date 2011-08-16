// spotlight.frag

#extension GL_EXT_gpu_shader4 : enable

uniform vec3	light_pos[2];
uniform vec4	light_diffuse[2];
uniform float	light_attConstant[2];
uniform float	light_attLinear[2];
uniform float	light_attQuadratic[2];

uniform vec3	light_direction[2];
uniform float	light_spotCutOff[2];

uniform int		light_type[2];		// 0 = omni, 1 = spotlight

uniform int		countLights = int(0);

uniform mat4	osg_ViewMatrix;

varying vec4 v;				// varying vertex in view space
varying vec3 n;

// good article about gl_NormalMatrix
// http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/

vec4 calcLighting( vec3 pos, float attConstant, float attLinear, float attQuadratic, vec4 lightDiffuse, float NdotL )
{
	float d = length(pos - v);	
	float att = 1.0 / (attConstant + attLinear*d + attQuadratic * (d*d));
	return (att * (lightDiffuse) ) * NdotL;				
}

vec4 calcLightingIntensity( vec4 v )
{
	vec4 c = vec4(0,0,0,0);
	
	// n is varying, so we need to normalize it here in the fragment shader
	vec3 normal = normalize( n );

	int i;
	for ( i=0; i<countLights; i++ )
	{
		vec3 test = light_pos[i];
	
		vec3 pos = osg_ViewMatrix * vec4( test, 1.0 );							// spot position in view space
		vec3 lightDir = normalize( pos - v );											// light direction to varying vertex
		
		float NdotL = max( dot(normal,lightDir), 0.0 );

		float attConstant	= light_attConstant[i];
		float attLinear		= light_attLinear[i];
		float attQuadratic	= light_attQuadratic[i];
		vec4 lightDiffuse	= light_diffuse[i];
		vec3 spotDir		= vec3(osg_ViewMatrix * vec4(light_direction[i],0.0));
		float cutOff		= light_spotCutOff[i];
		int type			= light_type[i];
		
		if ( type==0 )
		{
			c += calcLighting( pos, attConstant, attLinear, attQuadratic, lightDiffuse, NdotL );
		}
		else if ( type==1 )
		{
			float f = dot( spotDir, -lightDir );
			if (  f > cutOff )
			{	
				c += calcLighting( pos, attConstant, attLinear, attQuadratic, lightDiffuse, NdotL );
			}
		}
	}
	
	return c;
}

void main( void )
{
	gl_FragColor = vec4( 1,1,1,1 ) * calcLightingIntensity( v );
}
