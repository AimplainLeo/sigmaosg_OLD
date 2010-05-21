uniform sampler2D colorMap;
uniform sampler2D normalMap;

varying vec2 texCoord;
varying vec3 normal;

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

	// Ambient term is always present
	vec4 color = ambient;

	// Extract the object space normal from the texture and move it into view space
	vec3 n = gl_NormalMatrix * normalize(texture2D(normalMap, texCoord).xyz * 2.0 - 1.0);

	// NB! This is apparently needed on the hd2400xt I tested on,
	// even though gl_NormalMatrix should have preserved the length of the already normalized vector!
	n = normalize(n);
	
	// compute the dot product between normal and ldir
	float NdotL = max(dot(n,lightDir),0.0);
	
	// lookup the diffuse texture color
	vec4 diffuse_texture = texture2D(colorMap, texCoord);

	//vec4 diffuse_final = diffuse * diffuse_texture;
	vec4 diffuse_final = diffuse_texture;

	// compute the diffuse and specular terms and add them to the color
	if (NdotL > 0.0)
	{
		// diffuse
		color += diffuse_final * NdotL;

		// specular
		vec3 halfV = normalize(halfVec);
		float NdotHV = max(dot(n,halfV),0.0);
		color = color + gl_FrontMaterial.specular * gl_LightSource[i].specular * pow(NdotHV, gl_FrontMaterial.shininess);
	}

	gl_FragColor = color;
}
