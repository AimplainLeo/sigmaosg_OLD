// SulShaderBW.cpp

#include "stdafx.h"
#include "SulShaderBW.h"

CSulShaderBW::CSulShaderBW( osg::Node* pNode, osg::Program* pProg  ) :
CSulShaderBase( pNode )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/bw.frag" );
		addShaderVert( "shaders/bw.vert" );

		m_rUniform_invert = new osg::Uniform( osg::Uniform::BOOL, "bInvert" );
		m_rSS->addUniform( m_rUniform_invert.get() );
		m_rUniform_invert->set( false );
	}
}

void CSulShaderBW::setInvert( bool bInvert )
{
	m_rUniform_invert->set( bInvert );
}
