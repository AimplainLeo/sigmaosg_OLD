// SulShaderGaussV.cpp

#include "stdafx.h"
#include "SulShaderGaussV.h"

CSulShaderGaussV::CSulShaderGaussV( osg::Node* pNode, float fTexV, osg::Program* pProg ) :
CSulShaderBase( pNode )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/gaussv.frag" );
		addShaderVert( "shaders/gaussv.vert" );

		osg::Uniform* uniform_ftexv = new osg::Uniform( osg::Uniform::FLOAT, "fTexV" );
		uniform_ftexv->set( fTexV );
		m_rSS->addUniform( uniform_ftexv );
	}
}

