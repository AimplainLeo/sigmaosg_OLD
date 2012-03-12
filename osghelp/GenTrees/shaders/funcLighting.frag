// funcLighting.frag


// 1... we calculate for all lights,.. even a million miles away
// 2... we only support omni lights

#extension GL_EXT_gpu_shader4 : enable

uniform vec3	light_pos[14];
uniform vec4	light_diffuse[14];
uniform float	light_attConstant[14];
uniform float	light_attLinear[14];
uniform float	light_attQuadratic[14];

uniform vec3	light_direction[14];
uniform float	light_spotCutOff[14];

uniform int		light_type[14];			// 0 = omni, 1 = spotlight

uniform bool	light_enabled[14];
uniform int		light_id[14];

uniform int		countLights;

uniform mat4	osg_ViewMatrix;

uniform bool light4;					// we are using light4 for helmet 

vec4 calcLighting( vec3 pos, float attConstant, float attLinear, float attQuadratic, vec4 lightDiffuse, float NdotL, vec3 v )
{
	float d = length(pos - v);	
	float att = 1.0 / (attConstant + attLinear*d + attQuadratic * (d*d));
	return (att * (lightDiffuse) ) * NdotL;				
}

vec4 calcLightingIntensity( vec4 v, vec3 n )
{
	vec4 c = vec4(0.0,0.0,0.0,0.0);
	
	int i;
	for ( i=0; i<countLights; i++ )
	{
		// ignore if light is not on
		if ( !light_enabled[i] )
			continue;
			
		// special case, light4 is for the helment and can only be seen in nightvision
		if ( light_id[i]==4 && !light4 )
			continue;
	
		vec3 test = light_pos[i];
	
		vec3 pos = (osg_ViewMatrix * vec4( test, 1.0 )).xyz;							// spot position in view space
		//vec3 pos = test;
		vec3 lightDir = normalize( pos - v.xyz );										// light direction to varying vertex
		
		float NdotL = max( dot(n,lightDir), 0.0 );

		float attConstant	= light_attConstant[i];
		float attLinear		= light_attLinear[i];
		float attQuadratic	= light_attQuadratic[i];
		vec4 lightDiffuse	= light_diffuse[i];
		vec3 spotDir		= vec3(osg_ViewMatrix * vec4(light_direction[i],0.0));
		float cutOff		= light_spotCutOff[i];
		int type			= light_type[i];
		
		if ( type==0 )	// omni
		{
			c += calcLighting( pos, attConstant, attLinear, attQuadratic, lightDiffuse, NdotL, v.xyz );
		}
		else if ( type==1 ) // spotlight
		{
			// dot product between light and normal
			if ( NdotL > 0.0 )
			{
				float f = dot( spotDir, -lightDir );
				if (  f > cutOff )
				{	
					// do a linear fade to cutOff
					float m = (1.0/(1.0-cutOff));
					float test = m*f - (m*cutOff);
				
					c += calcLighting( pos, attConstant, attLinear, attQuadratic, lightDiffuse, NdotL, v.xyz ) * test;
//					c += vec4(1,0,0,1);
				}
			}
		}
	}
	
	return c;
}

