// SulShaderGaussH.cpp

#include "stdafx.h"
#include "SulShaderGaussH.h"

CSulShaderGaussH::CSulShaderGaussH( osg::Node* pNode, float fTexH, osg::Program* pProg ) :
CSulShaderBase( pNode )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/gaussh.frag" );
		addShaderVert( "shaders/gaussh.vert" );
		
		osg::Uniform* uniform_ftexh = new osg::Uniform( osg::Uniform::FLOAT, "fTexH" );
		uniform_ftexh->set( fTexH );
		m_rSS->addUniform( uniform_ftexh );	}
}