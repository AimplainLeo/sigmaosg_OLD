attribute vec3	vTangent;

varying vec2 texCoord;
varying vec3 normal;

varying vec3 lightDir;
varying vec3 halfVec;
varying vec3 eyeVec;

varying vec4 ambient;
varying vec4 diffuse;

uniform int	indexLight;

void main( void )
{
	// light source index
	int i = indexLight;

	// view space
	vec3 normalViewSpace = gl_NormalMatrix * normalize(gl_Normal.xyz);
	vec3 tangentViewSpace = gl_NormalMatrix * normalize(vTangent.xyz);
	vec3 binormalViewSpace = cross(normalViewSpace, tangentViewSpace);

	vec3 positionViewSpace = (gl_ModelViewMatrix * gl_Vertex).xyz;
	vec3 lightViewSpace = gl_LightSource[i].position.xyz;
	vec3 lightDirectionViewSpace;
	if ( gl_LightSource[i].position.w!=0 )
	{
		// point light
		lightDirectionViewSpace = normalize(lightViewSpace - positionViewSpace);
	}
	else
	{
		// directional light
		lightDirectionViewSpace = normalize(lightViewSpace);
	}

	//vec3 lightViewSpace = vec3(0,5,0);
	
	vec3 eyeViewSpace = normalize(-positionViewSpace);
	vec3 halfVecViewSpace = normalize(eyeViewSpace + lightDirectionViewSpace);

	// move to texture/tangent space
	mat3 viewToTextureSpaceMatrix = mat3(tangentViewSpace, binormalViewSpace, normalViewSpace);
	//mat3 viewToTextureSpaceMatrix = mat3(normalViewSpace, binormalViewSpace, tangentViewSpace);

	eyeVec = viewToTextureSpaceMatrix * eyeViewSpace;
	lightDir = viewToTextureSpaceMatrix * lightDirectionViewSpace;
	halfVec = viewToTextureSpaceMatrix * halfVecViewSpace;
	normal = viewToTextureSpaceMatrix * normalViewSpace;

	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[i].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[i].ambient;
	ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;

	gl_Position = ftransform();
	texCoord    = gl_MultiTexCoord0.xy;
	
	// trying out viewspace
	//lightDir = lightDirectionViewSpace;
	//normal = normalViewSpace;
	//halfVec = halfVecViewSpace;
}
