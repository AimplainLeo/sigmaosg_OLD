// SulShadeMono.cpp

#include "stdafx.h"
#include "SulShadeMono.h"

static const char fragShadeMono[] = 
	"uniform sampler2D tex; \n"
	"\n"
	"void main(void) \n"
    "{ \n"
	"	vec4 frag = texture2D( tex, gl_TexCoord[0] ); \n"
	"    gl_FragColor = frag*0.2; \n"
    "}\n";


void CSulShadeMono::set( osg::Node* pNode )
{
	osg::Shader* shaderFrag = new osg::Shader( osg::Shader::FRAGMENT, fragShadeMono );

    osg::Program* program = new osg::Program;
	program->addShader( shaderFrag );
	pNode->getOrCreateStateSet()->setAttribute( program );
}