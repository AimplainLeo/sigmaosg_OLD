// SulShaderCombineTex.cpp

#include "stdafx.h"
#include "SulShaderCombineTex.h"

CSulShaderCombineTex::CSulShaderCombineTex( osg::Node* pNode, osg::Texture* pTex0, osg::Texture* pTex1, osg::Program* pProg ) :
CSulShaderBase( pNode, pProg )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/combinetex.frag" );
		addShaderVert( "shaders/combinetex.vert" );

		osg::Uniform* uniform_tex0 = new osg::Uniform( osg::Uniform::SAMPLER_2D, "tex0" );
		uniform_tex0->set( 0 );		
		m_rSS->addUniform( uniform_tex0 );

		osg::Uniform* uniform_tex1 = new osg::Uniform( osg::Uniform::SAMPLER_2D, "tex1" );
		uniform_tex1->set( 1 );		
		m_rSS->addUniform( uniform_tex1 );

		m_rSS->setTextureAttribute( 0, pTex0 );
		m_rSS->setTextureAttribute( 1, pTex1 );
	}
}
