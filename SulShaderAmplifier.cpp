// SulShaderAmplifier.cpp

#include "stdafx.h"
#include "SulShaderAmplifier.h"

CSulShaderAmplifier::CSulShaderAmplifier( osg::Node* pNode, float fAmp, bool bColor, osg::Program* pProg  ) :
CSulShaderBase( pNode )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/amplifier.frag" );
		addShaderVert( "shaders/amplifier.vert" );

		m_rUniform_amp = new osg::Uniform( osg::Uniform::FLOAT, "fAmp" );
		m_rSS->addUniform( m_rUniform_amp );
		m_rUniform_amp->set( fAmp );

		m_rUniform_bcolor = new osg::Uniform( osg::Uniform::BOOL, "bColor" );
		m_rSS->addUniform( m_rUniform_bcolor );
		m_rUniform_bcolor->set( bColor );
	}
}

void CSulShaderAmplifier::setAmp( float val )
{
	m_rUniform_amp->set( val ) ;
}