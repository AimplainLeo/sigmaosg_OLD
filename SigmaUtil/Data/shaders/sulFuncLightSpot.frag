// sulFuncLightSpot.frag

// NOTE: ambient for the light sources are not currently supported

#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_texture_rectangle : enable

uniform int				countLightPoints;
uniform int				countLightSpots;
uniform sampler2DRect	texLightPoint;

uniform mat4			osg_ViewMatrix;

uniform bool light4;					// we are using light4 for helmet 

vec4 sulCalcLightingAttSpot( vec3 pos, float attConstant, float attLinear, float attQuadratic, vec4 lightDiffuse, float NdotL, vec3 v )
{
	float d = length(pos - v);	
	float att = 1.0 / (attConstant + attLinear*d + attQuadratic * (d*d));
	return (att * (lightDiffuse) ) * NdotL;				
}

vec4 sulCalcLightSpot( vec4 v, vec3 n )
{
	vec4 c = vec4(0.0,0.0,0.0,0.0);

	for ( int i=countLightPoints; i<countLightSpots+countLightPoints; i++ )
	{
		bool enabled = bool(texture2DRect( texLightPoint, vec2( 2, i ) ).b);
		if ( !enabled )
			continue;
	
		float id = texture2DRect( texLightPoint, vec2( 2, i ) ).a;	
	
		// special case, light4 is for the helment and can only be seen in nightvision
		if ( id==4 && !light4 )
			continue;

		vec3		pos;
		vec4		diffuseColor;
		vec4		ambientColor;
		float		attConstant;
		float		attLinear;
		float		attQuadratic;
		
		pos.x			= texture2DRect( texLightPoint, vec2( 0, i ) ).r;	
		pos.y			= texture2DRect( texLightPoint, vec2( 0, i ) ).g;	
		pos.z			= texture2DRect( texLightPoint, vec2( 0, i ) ).b;	
		diffuseColor.r	= texture2DRect( texLightPoint, vec2( 0, i ) ).a;	
		diffuseColor.g	= texture2DRect( texLightPoint, vec2( 1, i ) ).r;	
		diffuseColor.b	= texture2DRect( texLightPoint, vec2( 1, i ) ).g;	
		diffuseColor.a	= texture2DRect( texLightPoint, vec2( 1, i ) ).b;	
		attConstant		= texture2DRect( texLightPoint, vec2( 1, i ) ).a;	
		attLinear		= texture2DRect( texLightPoint, vec2( 2, i ) ).r;	
		attQuadratic	= texture2DRect( texLightPoint, vec2( 2, i ) ).g;	
		
		ambientColor.r	= texture2DRect( texLightPoint, vec2( 3, i ) ).r;	
		ambientColor.g	= texture2DRect( texLightPoint, vec2( 3, i ) ).g;	
		ambientColor.b	= texture2DRect( texLightPoint, vec2( 3, i ) ).b;	
		ambientColor.a	= texture2DRect( texLightPoint, vec2( 3, i ) ).a;	
		
		vec3 lightDir = normalize( pos - v.xyz );			// light direction from frag
		float NdotL = max( dot(n,lightDir), 0.0 );			// dot product between light and normal
		
		if ( NdotL > 0.0 )
		{
			vec3 spotDir;		// direction of spot in viewspace
			float cutOff;
			spotDir.x	= texture2DRect( texLightPoint, vec2( 4, i ) ).r;	
			spotDir.y	= texture2DRect( texLightPoint, vec2( 4, i ) ).g;	
			spotDir.z	= texture2DRect( texLightPoint, vec2( 4, i ) ).b;	
			cutOff		= texture2DRect( texLightPoint, vec2( 4, i ) ).a;	
		
			float f = dot( spotDir, -lightDir );
			if (  f > cutOff )
			{	
				// do a linear fade to cutOff
				float m = (1.0/(1.0-cutOff));
				float test = m*f - (m*cutOff);
			
				c+= sulCalcLightingAttSpot( pos, attConstant, attLinear, attQuadratic, diffuseColor, NdotL, v.xyz ) * test; 
				
				return vec4(1,0,0,1);
			}
		
		}
	}
		
	c.a = 1.0;
	return c;
}