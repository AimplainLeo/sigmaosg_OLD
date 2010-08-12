// SulGuiRadioButton.cpp

#include "stdafx.h"
#include "SulGuiRadioButton.h"
#include "SulGuiText.h"
#include "SulGeomLineStrip.h"
#include "SulGeomTriangleFan.h"
#include "SulGuiRadioButtonGroup.h"
#include "SulGuiEventHandler.h"

CSulGuiRadioButton::CSulGuiRadioButton( const CSulString& sText, float x, float y, float w, float h ) :
CSulGuiCanvas( "RADIOBUTTON", x, y, w!=0.0?w:128.0f, h!=0.0f?h:32.0f )
{
	sigma::VEC_VEC3::vector	vecPos;
	sigma::VEC_VEC3::vector	vecPosDots;
	
	float padding = 6.0f;

	double r = 16;
	double rdot = 13;

	for ( int i=0; i<17; i++ )
	{
		double d = ((2.0*osg::PI)/16.0) * (double)i;

		double x = cos( d );
		double y = sin( d );

		osg::Vec3 pos = osg::Vec3( x*r+w/2.0f, y*r+h/2.0f, 0 );
		vecPos.push_back( pos );

		osg::Vec3 posDot = osg::Vec3( x*rdot+w/2.0f, y*rdot+h/2.0f, 0 );
		vecPosDots.push_back( posDot );
	}

	// outer circle
	CSulGeomLineStrip* pLineStrip = new CSulGeomLineStrip( vecPos );	
	pLineStrip->setWidth( 2.0f );
	osg::Geode* pGeodeCircle = new osg::Geode;
	pGeodeCircle->addDrawable( pLineStrip->getDrawable() );
	addChild( pGeodeCircle );

	// text
	CSulGuiText* pGuiText = new CSulGuiText( sText, w+padding, h/2.0f, h );
	pGuiText->getTextObject()->setAlignment( osgText::TextBase::LEFT_CENTER );
	addChild( pGuiText );

	// inner dot
	CSulGeomTriangleFan* pTriangleFan = new CSulGeomTriangleFan( vecPosDots );
	m_rGeodeTriangleFan = new osg::Geode;
	m_rGeodeTriangleFan->addDrawable( pTriangleFan->getDrawable() );
	addChild( m_rGeodeTriangleFan );


	showCanvas( false );
}

void CSulGuiRadioButton::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );
	addEvent( CSulGuiEventHandler::EVENT_MOUSE_RELEASE );
}

void CSulGuiRadioButton::removeSelect()
{
	m_bActive = false;
	m_rGeodeTriangleFan->setNodeMask( 0 );
}

void CSulGuiRadioButton::setMouseRelease( bool bInside )
{
	if ( bInside )
	{
		// we need to disable any other radiobutton that is active in the same group
		CSulGuiRadioButtonGroup* pGroup = dynamic_cast<CSulGuiRadioButtonGroup*>(getParent(0));
		pGroup->removeSelect();
		m_rGeodeTriangleFan->setNodeMask( 0xFFFFFFFF );
		m_bActive = true;
	}
}
