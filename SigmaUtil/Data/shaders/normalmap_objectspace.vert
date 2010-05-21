varying vec3 normal;
varying vec2 texCoord;
varying vec3 lightDir;
varying vec3 halfVec;
varying vec3 eyeVec;

varying vec4 ambient;
varying vec4 diffuse;

uniform int indexLight;

void main( void )
{
	// light source index
	int i = indexLight;

	// compute all vectors in view/eye space
	vec3 normalViewSpace = gl_NormalMatrix * normalize(gl_Normal.xyz);

	vec3 positionViewSpace			= (gl_ModelViewMatrix * gl_Vertex).xyz;
	vec3 lightViewSpace				= gl_LightSource[i].position.xyz;
	vec3 lightDirectionViewSpace	= normalize(lightViewSpace - positionViewSpace);
	vec3 eyeViewSpace				= normalize(-positionViewSpace);
	vec3 halfVecViewSpace			= normalize(eyeViewSpace + lightDirectionViewSpace);

	// transfer varying values to fragment shader
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[i].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[i].ambient;
	ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;

	gl_Position = ftransform();
	texCoord = gl_MultiTexCoord0.xy;
	lightDir = lightDirectionViewSpace;
	normal = normalViewSpace;
	halfVec = halfVecViewSpace;
}
