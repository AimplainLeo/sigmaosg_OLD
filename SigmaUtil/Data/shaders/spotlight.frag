// spotlight.frag

#extension GL_EXT_gpu_shader4 : enable

uniform vec3	light_pos[10];
uniform vec4	light_diffuse[10];
uniform float	light_attConstant[10];
uniform float	light_attLinear[10];
uniform float	light_attQuadratic[10];
uniform float	light_id[10];
uniform float	light_enabled[10];

uniform vec3	light_direction[10];
uniform float	light_spotCutOff[10];

uniform int countLights = int(0);

uniform mat4		osg_ViewMatrix;

varying vec4 v;				// varying vertex in view space
varying vec3 n;

// good article about gl_NormalMatrix
// http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/

vec4 calcLightingIntensity( vec4 v )
{
	vec4 c = vec4(0,0,0,0);
	
	// n is varying, so we need to normalize it here in the fragment shader
	vec3 normal = normalize( n );

	int i;
	for ( i=0; i<countLights; i++ )
	{
		vec3 spotPos = osg_ViewMatrix * vec4( light_pos[i], 1.0 );							// spot position in view space
		vec3 lightDir = normalize( spotPos - v );											// light direction to varying vertex
		vec3 spotDir = gl_NormalMatrix * light_direction[i];
		
		float NdotL = max( dot(normal,lightDir), 0.0 );
		if ( NdotL > 0.0 )
		{
			float f = dot( spotDir, -lightDir );
			if (  f > light_spotCutOff[i] )
			{	
				float d = length(spotPos - v);	
				float att = 1.0 / (light_attConstant[i] + light_attLinear[i]*d + light_attQuadratic[i] * (d*d));
				c += att * (light_diffuse[i]);				
			}
		}
	}
	
	return c;
}

void main( void )
{
	gl_FragColor = vec4( 1,1,1,1 ) * calcLightingIntensity( v );
}
