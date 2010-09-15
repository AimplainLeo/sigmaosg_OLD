// SulShaderColor.cpp

#include "stdafx.h"
#include "SulShaderColor.h"

CSulShaderColor::CSulShaderColor( osg::Node* pNode, const osg::Vec4& color, osg::Program* pProg ) :
CSulShaderBase( pNode, pProg )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/color.frag" );

		m_rUniform_color = new osg::Uniform( osg::Uniform::FLOAT_VEC4, "color" );
		m_rSS->addUniform( m_rUniform_color.get() );
		m_rUniform_color->set( color );
	}
}