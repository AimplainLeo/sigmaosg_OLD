// main_standard.vert

uniform int use_tree_shader;

varying vec3 n;							// normal in camera-space
varying vec4 v;							// position in camera-space
varying vec4 vertexLightingColor;

// prototypes
vec4 calcTree();
vec4 calcTree2();
vec4 calcVertexLightingOpenGL( int i, vec4 vsVertexPosition );
vec4 calcBillboardLightingOpenGL(int i, vec4 vsVertexPosition);
vec4 calcBillboardLighting(vec4 vsVertexPosition);

// OpenGL gl_LightSource[i].position.w
// w = 0 : directional
// w = 1 : point light

// OpenGL Light Numbers:
// 2 : moon
// 3 : sun

varying vec3 lightDir2, halfVector2;
varying vec3 lightDir3, halfVector3;

varying vec4 ambient;

void DynamicShadow( in vec4 ecPosition )
{
    // generate coords for shadow mapping                          
    gl_TexCoord[3].s = dot( ecPosition, gl_EyePlaneS[3] );
    gl_TexCoord[3].t = dot( ecPosition, gl_EyePlaneT[3] );
    gl_TexCoord[3].p = dot( ecPosition, gl_EyePlaneR[3] );
    gl_TexCoord[3].q = dot( ecPosition, gl_EyePlaneQ[3] );
}

void main( void )
{
	// exactly why is this necessary?
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	// transform vertex from object-space to camera-space
	v = gl_ModelViewMatrix * gl_Vertex;

	// transform normal from object-space to camera-space
	n = normalize( gl_NormalMatrix * gl_Normal );   
	
	// MOON: directional light
	lightDir2 = normalize( gl_LightSource[2].position.xyz );
	halfVector2 = normalize( gl_LightSource[2].halfVector.xyz );

	// SUN: directional light
	lightDir3 = normalize( gl_LightSource[3].position.xyz );
	halfVector3 = normalize( gl_LightSource[3].halfVector.xyz );

	// ambient
	ambient = gl_LightModel.ambient + gl_LightSource[2].ambient + gl_LightSource[3].ambient;

	// billboard
	// instancing + quad alignment + vertex lighting
	if ( use_tree_shader==1 )
	{
		v = calcTree();
		vertexLightingColor = calcBillboardLightingOpenGL(2, v) + calcBillboardLightingOpenGL(3, v);
		//vertexLightingColor = calcBillboardLightingOpenGL(2, v) + calcBillboardLightingOpenGL(3, v) + calcBillboardLighting(v);
		//vertexLightingColor = calcBillboardLighting(v);
	}
	
	// cross tree
	// instancing + vertex lighting
	if ( use_tree_shader==2 )
	{
		v = calcTree2();
		vertexLightingColor = calcBillboardLightingOpenGL(2, v) + calcBillboardLightingOpenGL(3, v);
	}
	
	// cross grid
	// vertex lighting
	if ( use_tree_shader==3 )
	{
		vec4 posView = gl_ModelViewMatrix * gl_Vertex;
		vertexLightingColor = calcVertexLightingOpenGL( 3, posView );
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	}
	
	// normal
	if ( use_tree_shader==0 )
	{
		/*
		// shadow stuff
		vec4  ecPos  = gl_ModelViewMatrix * gl_Vertex;  
		DynamicShadow( ecPos );
		*/
		
		gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	}
} 
