// SulGuiButton.cpp

#include "stdafx.h"
#include "SulGuiButton.h"
#include "SulGeomTriangle.h"

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

void CSulGuiButton::createDownDecal()
{
	// create a triangle to represent an arrow down
	float pad = 10.0f;
	float w = getW();
	float h = getH();
	CSulGeomTriangle* pTriDown = new CSulGeomTriangle(
		osg::Vec3( w/2, h-pad, 0 ),
		osg::Vec3( pad, pad, 0 ),
		osg::Vec3( w-pad, pad, 0) );
	osg::Geode* pGeode = new osg::Geode;
	pGeode->addDrawable( pTriDown->getDrawable() );
	addChild( pGeode );
}

void CSulGuiButton::createUpDecal()
{
	// create a triangle to represent an arrow down
	float pad = 10.0f;
	float w = getW();
	float h = getH();
	CSulGeomTriangle* pTriDown = new CSulGeomTriangle(
		osg::Vec3( w/2, pad, 0 ),
		osg::Vec3( pad, h-pad, 0 ),
		osg::Vec3( w-pad, h-pad, 0) );
	osg::Geode* pGeode = new osg::Geode;
	pGeode->addDrawable( pTriDown->getDrawable() );
	addChild( pGeode );
}

void CSulGuiButton::setText( const CSulString& sText )
{
	m_sText = sText;
	if ( m_rText.valid() )
	{
		m_rText->setText( m_sText );
	}
}

const CSulString& CSulGuiButton::getText() const
{
	return m_sText;
}

void CSulGuiButton::setFontSize( float size )
{
	m_sizeFont = size;
	if ( m_rText.valid() )
	{
		m_rText->setFontSize( size );
	}
}

void CSulGuiButton::onHover( bool bHover )
{
	if ( isActive() )
	{
		setBgColor( bHover?osg::Vec4(1,0,0,0.5f):osg::Vec4(0,0,0,0.2f) );
	}
}

