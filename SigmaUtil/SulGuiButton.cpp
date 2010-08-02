// SulGuiButton.cpp

#include "stdafx.h"
#include "SulGuiButton.h"

CSulGuiButton::CSulGuiButton( const CSulString& sText, float x, float y, float w, float h ) :
CSulGuiCanvas( x, y, w!=0.0?w:128.0f, h!=0.0f?h:32.0f )
{
	addEvents( EVENT_MOUSE_MOVE );	
	//addEvents( EVENT_MOUSE_PUSHED );	
	//addEvents( EVENT_MOUSE_RELEASE );

	m_rText = new CSulGuiText( sText, 0, 0, 16.0f );
	m_rText->getTextObject()->setAlignment( osgText::TextBase::CENTER_CENTER );
	addChild( m_rText );

	// center text in button
	m_rText->setXY( getW()/2.0f, getH()/2.0f );
}

void CSulGuiButton::setMouseHover( bool bHover )
{
	CSulGuiCanvas::setMouseHover( bHover );

	setBgColor( bHover?osg::Vec4(1,0,0,0.5f):osg::Vec4(0,0,0,0.2f) );
}