// funcLighting.vert

uniform bool	light_enabled[14];
uniform int		light_id[14];
uniform vec3	light_pos[14];
uniform vec4	light_diffuse[14];
uniform float	light_attConstant[14];
uniform float	light_attLinear[14];
uniform float	light_attQuadratic[14];
uniform vec3	light_direction[14];
uniform float	light_spotCutOff[14];
uniform int		countLights;
uniform mat4	osg_ViewMatrix;
uniform bool	light4;					// we are using light4 for the helmet
uniform int		light_type[14];			// 0 = omni, 1 = spotlight

vec4 calcDiffuseLighting( vec3 pos, float attConstant, float attLinear, float attQuadratic, vec4 lightDiffuse, float NdotL, vec3 v )
{
	float d = length(pos - v);	
	float att = 1.0 / (attConstant + attLinear*d + attQuadratic * (d*d));
	return (att * (lightDiffuse) ) * NdotL;				
}

vec4 calcBillboardLighting(vec4 vsVertexPosition)
{
	vec4 color = vec4( 0.0, 0.0, 0.0, 1.0 );

	for ( int i = 0; i<countLights; i++ )
	{
		// ignore if light is not on
		if ( !light_enabled[i] )
			continue;
			
		// special case, light4 is for the helmet and can only be seen in nightvision
		if ( light_id[i]==4 && !light4 )
			continue;

		if ( light_type[i]!=1 )
			continue;
		
		vec3 spotDir		= vec3(osg_ViewMatrix * vec4(light_direction[i],0.0));
		
		vec4 vsLightPosition = osg_ViewMatrix * vec4(light_pos[i], 1.0);
		
		vec3 lightDir = normalize( vsLightPosition - vsVertexPosition.xyz );										// light direction to varying vertex
		vec3 n = vec3( 0.0, 0.0, 1.0 );
		float NdotL = max( dot(n,lightDir), 0.0 );

		if ( NdotL>0.0 )
		{
			float cutOff		= light_spotCutOff[i];
			
			float f = dot( spotDir, -lightDir );
			/*
			if (  f > cutOff )
			{
			*/	
				// do a linear fade to cutOff
				float m = (1.0/(1.0-cutOff));
				float test = m*f - (m*cutOff);
	
				
				
				if ( test>=0.0 )
				{
					if ( test>1.0 )
						test = 1.0;
				
					color += calcDiffuseLighting(vsLightPosition, light_attConstant[i], light_attLinear[i], light_attQuadratic[i], light_diffuse[i], NdotL, vsVertexPosition.xyz)*test;
				}
		//	}
		}
	}
	
//	return vec4( 1.0, 1.0, 1.0, 1.0 );
	return vec4(color.xyz, 1.0);
}
