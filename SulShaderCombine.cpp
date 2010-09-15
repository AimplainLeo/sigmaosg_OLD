// SulShaderCombine.cpp

#include "stdafx.h"
#include "SulShaderCombine.h"

CSulShaderCombine::CSulShaderCombine( osg::Node* pNode, float fExposure, float fBloomFactor, float fMaxBright, osg::Texture* pSrcColor, osg::Texture* pSrcBloom, osg::Program* pProg ) :
CSulShaderBase( pNode, pProg )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/compose.frag" );
		addShaderVert( "shaders/compose.vert" );

		m_rUniform_exposure = new osg::Uniform( osg::Uniform::FLOAT, "exposure" );
		m_rUniform_exposure->set( fExposure );
		m_rSS->addUniform( m_rUniform_exposure.get() );

		m_rUniform_bloomfactor = new osg::Uniform( osg::Uniform::FLOAT, "bloomFactor" );
		m_rUniform_bloomfactor->set( fBloomFactor );
		m_rSS->addUniform( m_rUniform_bloomfactor.get() );

		m_rUniform_maxbright = new osg::Uniform( osg::Uniform::FLOAT, "brightMax" );
		m_rUniform_maxbright->set( fMaxBright );
		m_rSS->addUniform( m_rUniform_maxbright.get() );

		osg::Uniform* uniform_srccolor = new osg::Uniform( osg::Uniform::SAMPLER_2D, "tex" );
		uniform_srccolor->set( 1 );		
		m_rSS->addUniform( uniform_srccolor );

		m_rUniform_bloom = new osg::Uniform( osg::Uniform::SAMPLER_2D, "bloom" );
		m_rUniform_bloom->set( 2 );		
		m_rSS->addUniform( m_rUniform_bloom.get() );

		m_rSS->setTextureAttribute( 1, pSrcColor );
		m_rSS->setTextureAttribute( 2, pSrcBloom );
	}
}

void CSulShaderCombine::setExposure( float f )
{
	m_rUniform_exposure->set( f );
}

void CSulShaderCombine::setFactor( float f )
{
	m_rUniform_bloomfactor->set( f );
}

void CSulShaderCombine::setMax( float f )
{
	m_rUniform_maxbright->set( f );
}

