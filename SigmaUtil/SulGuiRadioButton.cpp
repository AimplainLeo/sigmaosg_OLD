// SulGuiRadioButton.cpp

#include "stdafx.h"
#include "SulGuiRadioButton.h"
#include "SulGuiText.h"
#include "SulGeomLineStrip.h"
#include "SulGeomTriangleFan.h"

CSulGuiRadioButton::CSulGuiRadioButton( const CSulString& sText, float x, float y, float w, float h ) :
CSulGuiCanvas( x, y, w!=0.0?w:128.0f, h!=0.0f?h:32.0f )
{
	addEvents( EVENT_MOUSE_RELEASE );


	Sigma::VEC_VEC3::vector	vecPos;
	Sigma::VEC_VEC3::vector	vecPosDots;
	
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
	osg::Geode* pGeodeTriangleFan = new osg::Geode;
	pGeodeTriangleFan->addDrawable( pTriangleFan->getDrawable() );
	addChild( pGeodeTriangleFan );


	showCanvas( false );
}

void CSulGuiRadioButton::setMouseRelease( bool bInside )
{
	if ( bInside )
	{
		m_bActive = true;

		// we need to disable any other radiobutton that is active in the same group


//		m_rGeodeCross->setNodeMask( m_bActive?0xFFFFFFFF:0 );
	}
}
