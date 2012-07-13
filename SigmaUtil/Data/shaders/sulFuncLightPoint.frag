// sulFuncLightPoint.frag

// NOTE: ambient for the light sources are not currently supported

#extension GL_EXT_gpu_shader4 : enable
#extension GL_ARB_texture_rectangle : enable

uniform int				countLightPoints;
uniform sampler2DRect	texLightPoint;

uniform mat4			osg_ViewMatrix;

vec4 sulCalcLightingAtt( vec3 pos, float attConstant, float attLinear, float attQuadratic, vec4 lightDiffuse, float NdotL, vec3 v )
{
	float d = length(pos - v);	
	float att = 1.0 / (attConstant + attLinear*d + attQuadratic * (d*d));
	return (att * (lightDiffuse) ) * NdotL;				
}

vec4 sulCalcLightPoint( vec4 v, vec3 n, bool ignoreNormal )
{
	vec4 c = vec4(0.0,0.0,0.0,0.0);
	
	for ( int i=0; i<countLightPoints; i++ )
	{
		bool enabled = bool(texture2DRect( texLightPoint, vec2( 2, i ) ).b);
		if ( !enabled )
			continue;

		// id not currently used for anything
		float id = texture2DRect( texLightPoint, vec2( 2, i ) ).a;	

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
		
		vec3 posLight = pos;							// light position in view space
		
		vec3 lightDir = normalize( posLight - v.xyz );
		float NdotL = max( dot(n,lightDir), 0.0 );
		
		if ( ignoreNormal )
			NdotL = 1.0;		
		
		c+= sulCalcLightingAtt( posLight, attConstant, attLinear, attQuadratic, diffuseColor, NdotL, v.xyz ); 
	}
		
	c.a = 1.0;
	return c;
}