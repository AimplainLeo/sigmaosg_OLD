// SulGuiButton.cpp

#include "stdafx.h"
#include "SulGuiButton.h"
#include "SulGuiEventHandler.h"

CSulGuiButton::CSulGuiButton() :
CSulGuiCanvas( "BUTTON" )
{
}

CSulGuiButton::CSulGuiButton( const CSulString& sText, float x, float y, float w, float h ) :
CSulGuiCanvas( "BUTTON", x, y, w, h )
{
	m_sText = sText;
}

CSulGuiButton::CSulGuiButton( float x, float y, float w, float h ) :
CSulGuiCanvas( "BUTTON", x, y, w, h )
{
}

void CSulGuiButton::setupTheme( CSulGuiThemeXml* pThemeXml )
{
	CSulGuiCanvas::setupTheme( pThemeXml );

	m_sizeFont = getThemeValue( "font_size" ).asFloat();
}

void CSulGuiButton::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

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

	signalHover.connect( this, &CSulGuiButton::onHover );
}

void CSulGuiButton::onHover( bool bHover )
{
	if ( isActive() )
	{
		setBgColor( bHover?osg::Vec4(1,0,0,0.5f):osg::Vec4(0,0,0,0.2f) );
	}
}

