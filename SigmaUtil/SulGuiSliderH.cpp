// SulGuiSliderH.cpp

#include "stdafx.h"
#include "SulGuiSliderH.h"
#include "SulGeomLine.h"
#include "SulGuiButton.h"

CSulGuiSliderH::CSulGuiSliderH() :
CSulGuiCanvas( "SLIDERH" ),
m_size( 100.0f ),
m_value( 0.0f )
{
}

void CSulGuiSliderH::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	if ( pAttr->exist( "size" ) )	m_size = pAttr->get( "size" ).asFloat();
}

void CSulGuiSliderH::init()
{
	CSulGuiCanvas::init();

	// 1. need a horizontal line
	// 2. need a button slider

	showCanvas( false );

	setWH( m_size, 8.0f );

	CSulGeomLine* pLine = new CSulGeomLine( osg::Vec3(4,7,0), osg::Vec3(getW()+4,7,0) );
	pLine->setWidth( 2.0f );
	pLine->setColor( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	addChild( pLine );

	m_rSlider = new CSulGuiButton( 0, 0, 8, 8 );
	m_rSlider->setupEventHandler( getEventHandler() );
	m_rSlider->init();
	m_rSlider->createUpDecal();
	m_rSlider->showCanvas( false );
	m_rSlider->allowDrag( 0, getW(), 0, 0 );
	addChild( m_rSlider );

	// important that we are the first to be called, so we can save the value
	m_rSlider->signalPositionChanged.connect( this, &CSulGuiSliderH::onSliderChanged );
}

void CSulGuiSliderH::onSliderChanged( float x, float y )
{
	m_value = x/getW();
}

void CSulGuiSliderH::setValue( float v )
{
	m_value = v;
	m_rSlider->setX( v * getW() );
}

float CSulGuiSliderH::getValue()
{
	return m_value;
}

CSulGuiButton* CSulGuiSliderH::getSlider()
{
	return m_rSlider;
}