// SulGuiCheckBox.cpp

#include "stdafx.h"
#include "SulGuiCheckBox.h"
#include "SulGuiText.h"
#include "SulGeomLine.h"
#include "SulGuiEventHandler.h"
#include "SulGeomLineStrip.h"

CSulGuiCheckBox::CSulGuiCheckBox( const CSulString& sText, float x, float y, float w, float h ) :
CSulGuiCanvas( "CHECKBOX", x, y, w, h )
{
	m_sText = sText;
	m_bActive = false;
}

void CSulGuiCheckBox::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	m_paddingCross = getThemeValue( "padding_cross" ).asFloat();
	m_paddingText = getThemeValue( "padding_text" ).asFloat();
	m_fontSize = getThemeValue( "font_size" ).asFloat();
	m_boxSize = getThemeValue( "box_size" ).asFloat();

	if ( pAttr->exist( "padding_cross" ) )	m_paddingCross = pAttr->get( "padding_cross" ).asFloat();
	if ( pAttr->exist( "padding_text" ) )	m_paddingText = pAttr->get( "padding_Text" ).asFloat();
	if ( pAttr->exist( "font_size" ) )		m_fontSize = pAttr->get( "font_size" ).asFloat();
	if ( pAttr->exist( "box_size" ) )		m_boxSize = pAttr->get( "box_size" ).asFloat();
}

void CSulGuiCheckBox::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );
//	addEvent( CSulGuiEventHandler::EVENT_MOUSE_RELEASE );
}

void CSulGuiCheckBox::setMouseRelease( bool bInside )
{
	if ( bInside )
	{
		m_bActive = !m_bActive;
		m_rGeodeCross->setNodeMask( m_bActive?0xFFFFFFFF:0 );
	}
}

void CSulGuiCheckBox::init()
{
	CSulGuiCanvas::init();

	float w = getW();
	float h = getH();

	float d = (h-m_boxSize)/2.0f; // difference divided by 2 (used to place box and cross)

	CSulGeomLine* line0 = new CSulGeomLine( osg::Vec3(m_paddingCross, d+m_paddingCross, 0), osg::Vec3(m_boxSize-m_paddingCross, d+m_boxSize-m_paddingCross, 0) );
	line0->setWidth( 4.0f );

	CSulGeomLine* line1 = new CSulGeomLine( osg::Vec3(m_paddingCross, d+m_boxSize-m_paddingCross, 0), osg::Vec3(m_boxSize-m_paddingCross, d+m_paddingCross, 0) );
	line1->setWidth( 4.0f );

	m_rGeodeCross = new osg::Geode;
	m_rGeodeCross->addDrawable( line0->getDrawable() );
	m_rGeodeCross->addDrawable( line1->getDrawable() );
	m_rGeodeCross->setNodeMask( 0 );
	addChild( m_rGeodeCross );

	// draw a rectangle
	sigma::VEC_VEC3::vector	vecPos;

	vecPos.push_back( osg::Vec3(0,d,0) );
	vecPos.push_back( osg::Vec3(0,d+m_boxSize,0) );
	vecPos.push_back( osg::Vec3(m_boxSize,d+m_boxSize,0) );
	vecPos.push_back( osg::Vec3(m_boxSize,d,0) );
	vecPos.push_back( osg::Vec3(0,d,0) );

	CSulGeomLineStrip* pLineStrip = new CSulGeomLineStrip( vecPos );	
	pLineStrip->setWidth( 2.0f );
	osg::Geode* pGeodeBox = new osg::Geode;
	pGeodeBox->addDrawable( pLineStrip->getDrawable() );
	addChild( pGeodeBox );

	// text
	CSulGuiText* pGuiText = new CSulGuiText( m_sText, m_boxSize+m_paddingText, h/2.0f, m_fontSize );
	pGuiText->init();
	pGuiText->getTextObject()->setAlignment( osgText::TextBase::LEFT_CENTER );
	addChild( pGuiText );

	showCanvas( false );
}
