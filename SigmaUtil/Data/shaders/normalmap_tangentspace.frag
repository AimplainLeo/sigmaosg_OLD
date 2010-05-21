uniform sampler2D colorMap;
uniform sampler2D normalMap;

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

	/* The ambient term will always be present */
	//vec4 color = ambient;
	vec4 color = vec4(0,0,0,1);
	
	/* a fragment shader can't write a varying variable, hence we need
	a new variable to store the normalized interpolated normal */
	//vec3 n = normalize(normal);
	vec3 n = normalize(texture2D(normalMap, texCoord).xyz * 2.0 - 1.0);

	/* compute the dot product between normal and ldir */
	float NdotL = max(dot(n,lightDir),0.0);
	
	/* override the diffuse color with a diffuse texture lookup */
	vec4 diffuse_final = texture2D(colorMap, texCoord) * gl_FrontMaterial.diffuse;
	//vec4 diffuse_final = diffuse;
	//vec4 diffuse_final = vec4(1,1,1,1);

	/* compute the diffuse and specular terms and add them to the color */	
	if (NdotL > 0.0)
	{
		color += diffuse_final * NdotL;

		// specular term disabled due to incorrect specular and shininess values in the objects
		//float halfV = normalize(halfVec);
		//float NdotHV = max(dot(n,halfV),0.0);
		//color += gl_FrontMaterial.specular * 
		//		gl_LightSource[i].specular * 
		//		pow(NdotHV, gl_FrontMaterial.shininess);
		//color += 1.0 * pow(NdotHV, 1.0);
	}

	gl_FragColor = color;
	//gl_FragColor = vec4( 0.0, 0.0, gl_LightSource[i].position.w, 1.0 );
//	gl_FragColor = texture2D(normalMap, texCoord);
	//gl_FragColor = vec4(1,0,0,1);
}
