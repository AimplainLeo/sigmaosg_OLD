// SulGeodeCircle.cpp

#include "stdafx.h"
#include "SulGeodeCircle.h"

CSulGeodeCircle::CSulGeodeCircle( float radius ) :
CSulGeode(),
m_radius( radius )
{
	initConstructor();
}

void CSulGeodeCircle::initConstructor()
{
	m_geomCircle = new CSulGeomCircle( m_radius );
	addDrawable( m_geomCircle );
}

void CSulGeodeCircle::setOffset( const osg::Vec3& ofs )
{
	m_geomCircle->setOffset( ofs );
}
