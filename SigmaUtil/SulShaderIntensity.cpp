// SulShaderIntensity.cpp

#include "stdafx.h"
#include "SulShaderIntensity.h"


CSulShaderIntensity::CSulShaderIntensity( osg::Node* pNode, float fIntensity, float fMultiIntR, float fMultiIntG, float fMultiIntB, osg::Program* pProg ) :
CSulShaderBase( pNode )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/intensity.frag" );
		addShaderVert( "shaders/intensity.vert" );
	
		m_rUniform_multiR = new osg::Uniform( osg::Uniform::FLOAT, "multiR" );
		m_rUniform_multiR->set( fMultiIntR );
		m_rSS->addUniform( m_rUniform_multiR );

		m_rUniform_multiG = new osg::Uniform( osg::Uniform::FLOAT, "multiG" );
		m_rUniform_multiG->set( fMultiIntG );
		m_rSS->addUniform( m_rUniform_multiG );

		m_rUniform_multiB = new osg::Uniform( osg::Uniform::FLOAT, "multiB" );
		m_rUniform_multiB->set( fMultiIntB );
		m_rSS->addUniform( m_rUniform_multiB );

		m_rUniform_intensity = new osg::Uniform( osg::Uniform::FLOAT, "intensity" );
		m_rUniform_intensity->set( fIntensity );
		m_rSS->addUniform( m_rUniform_intensity );
	}
}

void CSulShaderIntensity::setMultiR( float f )
{
	m_rUniform_multiR->set( f );
}

void CSulShaderIntensity::setMultiG( float f )
{
	m_rUniform_multiG->set( f );
}

void CSulShaderIntensity::setMultiB( float f )
{
	m_rUniform_multiB->set( f );
}

void CSulShaderIntensity::setIntensity( float f )
{
	m_rUniform_intensity->set( f );
}