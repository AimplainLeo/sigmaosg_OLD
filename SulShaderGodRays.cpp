// SulShaderGodRays.cpp

#include "stdafx.h"
#include "SulShaderGodRays.h"

CSulShaderGodRays::CSulShaderGodRays( osg::Node* pNode, osg::Program* pProg ) :
CSulShaderBase( pNode )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/godrays.frag" );
		addShaderVert( "shaders/godrays.vert" );

		m_rUniform_exposure = new osg::Uniform( osg::Uniform::FLOAT, "exposure" );
		m_rSS->addUniform( m_rUniform_exposure );
		m_rUniform_exposure->set( 0.1f );

		m_rUniform_decay = new osg::Uniform( osg::Uniform::FLOAT, "decay" );
		m_rSS->addUniform( m_rUniform_decay );
		m_rUniform_decay->set( 0.95f );

		m_rUniform_density = new osg::Uniform( osg::Uniform::FLOAT, "density" );
		m_rSS->addUniform( m_rUniform_density );
		m_rUniform_density->set( 1.22f );

		m_rUniform_weight = new osg::Uniform( osg::Uniform::FLOAT, "weight" );
		m_rSS->addUniform( m_rUniform_weight );
		m_rUniform_weight->set( 0.55f );
	}
}

void CSulShaderGodRays::setExposure( float exposure )
{
	m_rUniform_exposure->set( exposure );
}

void CSulShaderGodRays::setDecay( float decay )
{
	m_rUniform_decay->set( decay );
}

void CSulShaderGodRays::setDensity( float density )
{
	m_rUniform_density->set( density );
}

void CSulShaderGodRays::setWeight( float weight )
{
	m_rUniform_weight->set( weight );
}