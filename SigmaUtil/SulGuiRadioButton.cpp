// SulGuiRadioButton.cpp

#include "stdafx.h"
#include "SulGuiRadioButton.h"
#include "SulGuiText.h"
#include "SulGeodeLineStrip.h"
#include "SulGuiRadioButtonGroup.h"
#include "SulGuiEventHandler.h"

CSulGuiRadioButton::CSulGuiRadioButton( const CSulString& sText, float x, float y, float w, float h ) :
CSulGuiCanvas( "RADIOBUTTON", x, y, w, h )
{
	m_sText = sText;
}

void CSulGuiRadioButton::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	m_fontSize = getThemeValue( "font_size" ).asFloat();
	m_radioSizeOuter = getThemeValue( "radio_size_outer" ).asFloat();
	m_radioSizeInner = getThemeValue( "radio_size_inner" ).asFloat();
	m_paddingText = getThemeValue( "padding_text" ).asFloat();

	if ( pAttr->exist( "font_size" ) )			m_fontSize = pAttr->get( "font_size" ).asFloat();
	if ( pAttr->exist( "radio_size_outer" ) )	m_radioSizeOuter = pAttr->get( "radio_size_outer" ).asFloat();
	if ( pAttr->exist( "radio_size_inner" ) )	m_radioSizeInner = pAttr->get( "radio_size_inner" ).asFloat();
	if ( pAttr->exist( "padding_text" ) )		m_paddingText = pAttr->get( "padding_text" ).asFloat();
}

void CSulGuiRadioButton::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );
//	addEvent( CSulGuiEventHandler::EVENT_MOUSE_RELEASE );
}

void CSulGuiRadioButton::init()
{
	CSulGuiCanvas::init();

	sigma::VEC_VEC3::vector	vecPos;
	sigma::VEC_VEC3::vector	vecPosDots;
	
	float w = getW();
	float h = getH();

	double r = m_radioSizeOuter;
	double rdot = m_radioSizeInner;

	for ( int i=0; i<17; i++ )
	{
		double d = ((2.0*osg::PI)/16.0) * (double)i;

		double x = cos( d );
		double y = sin( d );

		osg::Vec3 pos = osg::Vec3( x*r+r, y*r+h/2.0f, 0 );
		vecPos.push_back( pos );

		osg::Vec3 posDot = osg::Vec3( x*rdot+r, y*rdot+h/2.0f, 0 );
		vecPosDots.push_back( posDot );
	}

	// outer circle
	CSulGeodeLineStrip* pLineStrip = new CSulGeodeLineStrip( vecPos );	
	pLineStrip->setWidth( 2.0f );
	addChild( pLineStrip );

	// text
	CSulGuiText* pGuiText = new CSulGuiText( m_sText, r*2.0f+m_paddingText, h/2.0f, m_fontSize );
	pGuiText->init();
	pGuiText->getTextObject()->setAlignment( osgText::TextBase::LEFT_CENTER );
	addChild( pGuiText );

	// inner dot
	m_rTriangleFan = new CSulGeomTriangleFan( vecPosDots );
	addChild( m_rTriangleFan );

	showCanvas( false );
}

void CSulGuiRadioButton::removeSelect()
{
	m_bActive = false;
	m_rTriangleFan->setNodeMask( 0 );
}

void CSulGuiRadioButton::setMouseRelease( bool bInside )
{
	if ( bInside )
	{
		// we need to disable any other radiobutton that is active in the same group
		CSulGuiRadioButtonGroup* pGroup = dynamic_cast<CSulGuiRadioButtonGroup*>(getParent(0));
		pGroup->removeSelect();
		m_rTriangleFan->setNodeMask( 0xFFFFFFFF );
		m_bActive = true;
	}
}
