// SulShaderGuiFrame.cpp

#include "stdafx.h"
#include "SulShaderGuiFrame.h"

CSulShaderGuiFrame::CSulShaderGuiFrame( osg::Node* pNode, osg::Program* pProg ) :
CSulShaderBase( pNode )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/gui_frame.frag" );
		addShaderVert( "shaders/gui_frame.vert" );

		m_rUniform_w = new osg::Uniform( osg::Uniform::FLOAT, "w" );
		//m_rUniform_w->set( fMultiIntR );
		m_rSS->addUniform( m_rUniform_w );

		m_rUniform_h = new osg::Uniform( osg::Uniform::FLOAT, "h" );
		//m_rUniform_w->set( fMultiIntR );
		m_rSS->addUniform( m_rUniform_h );

		m_rUniform_border = new osg::Uniform( osg::Uniform::FLOAT, "border" );
		//m_rUniform_w->set( fMultiIntR );
		m_rSS->addUniform( m_rUniform_border );


	}
}
