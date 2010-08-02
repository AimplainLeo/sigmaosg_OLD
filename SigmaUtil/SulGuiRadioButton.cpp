// SulGuiRadioButton.cpp

#include "stdafx.h"
#include "SulGuiRadioButton.h"
#include "SulGuiText.h"
#include "SulGeomLine.h"
#include "SulGeomLineList.h"

CSulGuiRadioButton::CSulGuiRadioButton( const CSulString& sText, float x, float y, float w, float h ) :
CSulGuiCanvas( x, y, w!=0.0?w:128.0f, h!=0.0f?h:32.0f )
{
	addEvents( EVENT_MOUSE_RELEASE );

	float padding = 6.0f;

	CSulGeomLine* line0 = new CSulGeomLine( osg::Vec3(padding, padding, 0), osg::Vec3(w-padding, h-padding, 0) );
	line0->setWidth( 4.0f );

	CSulGeomLine* line1 = new CSulGeomLine( osg::Vec3(padding, h-padding, 0), osg::Vec3(w-padding, padding, 0) );
	line1->setWidth( 4.0f );

	m_rGeodeCross = new osg::Geode;
	m_rGeodeCross->addDrawable( line0->getDrawable() );
	m_rGeodeCross->addDrawable( line1->getDrawable() );
	m_rGeodeCross->setNodeMask( 0 );
	addChild( m_rGeodeCross );

	CSulGuiText* pGuiText = new CSulGuiText( sText, w, h/2.0f, h );
	pGuiText->getTextObject()->setAlignment( osgText::TextBase::LEFT_CENTER );
	addChild( pGuiText );


	Sigma::VEC_LINESEGMENT::vector	vecLines;

	double r = 16;

	for ( int i=0; i<16; i++ )
	{
		double d = ((2.0*osg::PI)/16.0) * (double)i;

		double x = cos( d )*r;
		double y = sin( d )*r;

//		osg::Vec3 pos = osg::Vec3( x, y, 0 );
	}

	CSulGeomLineList* pLineList = new CSulGeomLineList( vecLines );
	osg::Geode* pGeodeCircle = new osg::Geode;
	pGeodeCircle->addDrawable( pLineList->getDrawable() );
	addChild( pGeodeCircle );
}

void CSulGuiRadioButton::setMouseRelease( bool bInside )
{
	if ( bInside )
	{
		m_bActive = true;
		m_rGeodeCross->setNodeMask( m_bActive?0xFFFFFFFF:0 );
	}
}
