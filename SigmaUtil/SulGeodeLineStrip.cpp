// SulGeodeLineStrip.cpp

#include "stdafx.h"
#include "SulGeodeLineStrip.h"
#include <osg/geometry>

CSulGeodeLineStrip::CSulGeodeLineStrip() :
CSulGeode()
{
	initConstructor();
}

CSulGeodeLineStrip::CSulGeodeLineStrip( const sigma::VEC_VEC3& vecVector ) :
CSulGeode(),
m_vecVector( vecVector )
{
	initConstructor();
}

void CSulGeodeLineStrip::initConstructor()
{
	m_geomLineStrip = new CSulGeomLineStrip( m_vecVector );
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
