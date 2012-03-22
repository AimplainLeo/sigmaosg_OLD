// main_standard.frag

varying vec3			n;
varying vec4			v;
varying vec4			vertexLightingColor;

uniform sampler2D		tex;
uniform sampler2DShadow osgShadow_shadowTexture;

uniform bool			bUseIntensityAmp;		// use the intensityAmp value
uniform float			intensityAmp;			// intensity value to apply on frag
uniform float			intensityAmpTree;		// intensity value for tree processing

uniform int			use_tree_shader;		// true if processing trees

uniform bool			bThermal;				// true if thermal is active

// prototypes
vec4 calcLightingAmbientIntensityOpenGL();
vec4 calcFogLinear( vec4 fragIn );
vec3 calcLightingIntensityOpenGL_new( int i, vec4 v, vec3 n, vec3 lightDir );
vec4 calcLightingIntensity(  vec4 v, vec3 n );

// OpenGL Light Numbers:
// 2 : moon
// 3 : sun

varying vec3 lightDir2, halfVector2;
varying vec3 lightDir3, halfVector3;

varying vec4 ambient;

vec4 calcPerPixelLighting( vec4 frag )
{
	// get lighting intensity
	vec4 vDiffuse, vAmbient;

	// create a new variable because we can not change the varying n variable
	vec3 nn;
	nn = normalize( n );
	
	//vAmbient = calcLightingAmbientIntensityOpenGL() + gl_LightSource[2].ambient + gl_LightSource[3].ambient;
	vec4 iD = calcLightingIntensity( v, nn );
	vec3 iD2 = calcLightingIntensityOpenGL_new( 2, v, nn, lightDir2 );
	vec3 iD3 = calcLightingIntensityOpenGL_new( 3, v, nn, lightDir3 );

	vDiffuse = iD + vec4( iD2+iD3, 1.0 );
	
	// calculate color with intensity	
	return frag * vDiffuse;// + gl_FrontMaterial.ambient * vAmbient;
}

void main( void )
{

	vec4 frag = texture2D( tex, gl_TexCoord[0].st );

	gl_FragData[0] = frag;
	return;

	if ( use_tree_shader==1 )
	{
		frag = frag*vertexLightingColor;
		frag = calcFogLinear( frag );
	}
	
	if ( use_tree_shader==2 )
	{
	}
	
	if ( use_tree_shader==0 )
	{
		if ( !bThermal )
		{
			frag = calcPerPixelLighting( frag );
			frag = calcFogLinear( frag );
		}
	}

	if ( bUseIntensityAmp )
	{
		if ( use_tree_shader )
			frag *= intensityAmpTree;
		else
			frag *= intensityAmp;
	}
	
	/*
		vec4 shadow = shadow2DProj( osgShadow_shadowTexture, gl_TexCoord[3] );
		if ( shadow.r<0.9 )
		{
			frag.rgb = frag.rgb * 0.6;
		}
	*/

	//frag += ambient;
	
	
//frag = vec4( 1.0, 0.0, 0.0, 1.0 );	
	
	gl_FragData[0] = frag;
	gl_FragData[1] = vec4(-v.z, 0.0, 0.0, 0.0);
	
//	gl_FragData[0] = vec4(1.0,0.0, 0.0, 1.0);

} 
