// SulGuiDial.cpp

#include "stdafx.h"
#include "SulGuiDial.h"
#include "SulGeomCircle.h"
#include "SulGeomArrow2D.h"
#include "SulGuiEventHandler.h"
#include "SulMath.h"
#include <iostream>

CSulGuiDial::CSulGuiDial() :
CSulGuiCanvas( "DIAL" )
{
	m_radius = 32.0f;
	m_deg = 0.0f;
}

CSulGuiDial::CSulGuiDial( float radius ) :
CSulGuiCanvas( "DIAL", 0, 0, radius, radius )
{
	m_radius = radius;
	m_deg = 0.0f;
}

void CSulGuiDial::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	if ( pAttr->exist( "radius" ) )	m_radius = pAttr->get( "radius" ).asFloat();
}

void CSulGuiDial::init()
{
	CSulGuiCanvas::init();

	setWH( m_radius*2.0f, m_radius*2.0f );

	CSulGeomCircle* pCircle = new CSulGeomCircle( m_radius );
	pCircle->setOffset( osg::Vec3(m_radius, m_radius, 0.0f) );
	osg::Geode* pGeodeCircle = new osg::Geode;
	pGeodeCircle->addDrawable( pCircle->getDrawable() );
	addChild( pGeodeCircle );

	m_mt = new osg::MatrixTransform;
	osg::Matrix m;
	m.setTrans( osg::Vec3(m_radius,m_radius,0) );
	m_mt->setMatrix( m );
	CSulGeomArrow2D* pArrow = new CSulGeomArrow2D( m_radius, 10.0f, 10.0f );
	m_mt->addChild( pArrow->createGeode() );
	addChild( m_mt );

	getEventHandler()->wantEvent( this, this, CSulGuiEventHandler::EVENT_MOUSERELEASE ); 

	showCanvas( false );
}

void CSulGuiDial::eventMouseRelease( CSulGuiCanvas* pCanvas, float local_x, float local_y, float x, float y )
{
	// NOTE: because we have flipped the y-axis to have 0,0 in top,left corner.. means that the degress for
	// circle is also flipped on the y-axis. which means that our angle calculation is going clockwise.
	// we need to fix this when presenting this value to the user of this class.
	float w = getW();
	float h = getH();

	float xx = local_x - (w/2.0f);
	float yy = local_y-(h/2.0f);

	float a = atan2( yy, xx );
	
	m_deg = sigma::RAD2DEG( a );

	if ( m_deg<0 )
		m_deg = 180.0f + (180.0f+m_deg);

	osg::Quat q( sigma::DEG2RAD(m_deg), osg::Vec3(0,0,1) );
	osg::Matrix m = m_mt->getMatrix();
	m.setRotate( q );
	m_mt->setMatrix( m );
}

osg::Vec2 CSulGuiDial::getDir()
{
	float r = sigma::DEG2RAD(m_deg);

	return osg::Vec2( cos(r), -sin(r) );
}

