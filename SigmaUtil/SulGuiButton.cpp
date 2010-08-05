// SulGuiButton.cpp

#include "stdafx.h"
#include "SulGuiButton.h"
#include "SulGuiEventHandler.h"

CSulGuiButton::CSulGuiButton( const CSulString& sText, float x, float y, float w, float h ) :
CSulGuiCanvas( x, y, w!=0.0?w:128.0f, h!=0.0f?h:32.0f )
{
	m_rText = new CSulGuiText( sText, 0, 0, 16.0f );
	m_rText->getTextObject()->setAlignment( osgText::TextBase::CENTER_CENTER );
	m_rText->setXY( getW()/2.0f, getH()/2.0f );
	addChild( m_rText );

	init();
}

CSulGuiButton::CSulGuiButton( float x, float y, float w, float h ) :
CSulGuiCanvas( x, y, w, h )
{
	init();
}

void CSulGuiButton::init()
{
	m_test = 0;
}

void CSulGuiButton::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );

	addEvent( CSulGuiEventHandler::EVENT_MOUSE_MOVE );	
	//addEvents( EVENT_MOUSE_PUSHED );	
	addEvent( CSulGuiEventHandler::EVENT_MOUSE_RELEASE );
}

void CSulGuiButton::setMouseHover( bool bHover )
{
	CSulGuiCanvas::setMouseHover( bHover );

	setBgColor( bHover?osg::Vec4(1,0,0,0.5f):osg::Vec4(0,0,0,0.2f) );
}

void CSulGuiButton::setMouseRelease( bool bInside )
{
	if ( bInside && m_test )
		m_test->click( this );
}

void CSulGuiButton::addMediator( CSulGuiButtonMediatorBase* p )
{
	m_test = p;
}