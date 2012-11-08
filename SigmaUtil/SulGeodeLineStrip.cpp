// SulGeodeLineStrip.cpp

#include "stdafx.h"
#include "SulGeodeLineStrip.h"
#include <osg/geometry>

CSulGeodeLineStrip::CSulGeodeLineStrip() :
CSulGeode(),
m_color( osg::Vec4(1,1,1,1) )
{
	initConstructor();
}

CSulGeodeLineStrip::CSulGeodeLineStrip( const sigma::VEC_VEC3& vecVector, const osg::Vec4& color ) :
CSulGeode(),
m_vecVector( vecVector ),
m_color( color )
{
	initConstructor();
}

void CSulGeodeLineStrip::initConstructor()
{
	m_geomLineStrip = new CSulGeomLineStrip( m_vecVector, m_color );
	addDrawable( m_geomLineStrip );
}

void CSulGeodeLineStrip::setLines( const sigma::VEC_VEC3& vecVector )
{
	m_geomLineStrip->setLines( vecVector );
}

void CSulGeodeLineStrip::setWidth( float width )
{
	m_geomLineStrip->setWidth( width );
}
