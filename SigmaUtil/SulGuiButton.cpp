// SulGuiButton.cpp

#include "stdafx.h"
#include "SulGuiButton.h"
#include "SulGuiEventHandler.h"

CSulGuiButton::CSulGuiButton( const CSulString& sText, float x, float y, float w, float h ) :
CSulGuiCanvas( "BUTTON", x, y, w, h )
{
	m_sText = sText;
}

CSulGuiButton::CSulGuiButton( float x, float y, float w, float h ) :
CSulGuiCanvas( "BUTTON", x, y, w, h )
{
}

void CSulGuiButton::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	m_sizeFont = getThemeValue( "font_size" ).asFloat();

	if ( pAttr->exist( "font_size" ) ) m_sizeFont = pAttr->get( "font_size" ).asFloat();
}

void CSulGuiButton::init()
{
	CSulGuiCanvas::init();

	m_rText = new CSulGuiText( m_sText, 0, 0, m_sizeFont );
	m_rText->init();
	m_rText->getTextObject()->setAlignment( osgText::TextBase::CENTER_CENTER );
	m_rText->setXY( getW()/2.0f, getH()/2.0f );
	addChild( m_rText );
}

void CSulGuiButton::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );

	addEvent( CSulGuiEventHandler::EVENT_MOUSE_MOVE );	
	addEvent( CSulGuiEventHandler::EVENT_MOUSE_RELEASE );
}

void CSulGuiButton::setMouseHover( bool bHover )
{
	CSulGuiCanvas::setMouseHover( bHover );

	if ( isActive() )
	{
		setBgColor( bHover?osg::Vec4(1,0,0,0.5f):osg::Vec4(0,0,0,0.2f) );
	}
}

void CSulGuiButton::setMouseRelease( bool bInside )
{

}

